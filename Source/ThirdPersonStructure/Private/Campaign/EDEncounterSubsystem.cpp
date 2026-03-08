#include "Campaign/EDEncounterSubsystem.h"
#include "Campaign/EDCampaignStateSubsystem.h"
#include "Campaign/EDFactionSubsystem.h"
#include "Data/EncounterDataAsset.h"

void UEDEncounterSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    RandomStream.Initialize(FMath::Rand());
}

void UEDEncounterSubsystem::SetEncounterDataAsset(UEncounterDataAsset* DataAsset)
{
    ActiveEncounterData = DataAsset;
}

FEncounterDefinition UEDEncounterSubsystem::GenerateEncounter()
{
    TArray<FEncounterDefinition> ValidEncounters = FilterValidEncounters();

    if (ValidEncounters.Num() == 0)
    {
        return FEncounterDefinition();
    }

    FEncounterDefinition Selected = SelectWeightedEncounter(ValidEncounters);
    OnEncounterGenerated.Broadcast(Selected);
    return Selected;
}

TArray<FEncounterDefinition> UEDEncounterSubsystem::GenerateEncounterPool(int32 Count)
{
    TArray<FEncounterDefinition> Pool;
    TArray<FEncounterDefinition> ValidEncounters = FilterValidEncounters();

    if (ValidEncounters.Num() == 0)
    {
        return Pool;
    }

    Count = FMath::Min(Count, ValidEncounters.Num());

    TArray<FEncounterDefinition> Remaining = ValidEncounters;
    for (int32 i = 0; i < Count && Remaining.Num() > 0; ++i)
    {
        FEncounterDefinition Selected = SelectWeightedEncounter(Remaining);
        Pool.Add(Selected);
        Remaining.RemoveAll([&Selected](const FEncounterDefinition& E)
        {
            return E.EncounterId == Selected.EncounterId;
        });
    }

    return Pool;
}

void UEDEncounterSubsystem::MarkEncounterCompleted(FName EncounterId)
{
    CompletedEncounters.AddUnique(EncounterId);
}

bool UEDEncounterSubsystem::HasCompletedEncounter(FName EncounterId) const
{
    return CompletedEncounters.Contains(EncounterId);
}

void UEDEncounterSubsystem::SetCompletedEncounters(const TArray<FName>& Encounters)
{
    CompletedEncounters = Encounters;
}

TArray<FEncounterDefinition> UEDEncounterSubsystem::FilterValidEncounters() const
{
    TArray<FEncounterDefinition> Result;

    if (!ActiveEncounterData)
    {
        return Result;
    }

    const UGameInstance* GI = GetGameInstance();
    if (!GI)
    {
        return Result;
    }

    const UEDCampaignStateSubsystem* CampaignState = GI->GetSubsystem<UEDCampaignStateSubsystem>();
    if (!CampaignState)
    {
        return Result;
    }

    const int32 Chapter = CampaignState->CurrentChapter;
    const EPrinceId Prince = CampaignState->SelectedPrince;
    const ERoutePhase Phase = CampaignState->CurrentPhase;

    for (const FEncounterDefinition& Encounter : ActiveEncounterData->Encounters)
    {
        if (CompletedEncounters.Contains(Encounter.EncounterId))
        {
            continue;
        }

        if (Chapter < Encounter.MinChapter || Chapter > Encounter.MaxChapter)
        {
            continue;
        }

        if (Encounter.RequiredPhase != Phase)
        {
            continue;
        }

        if (Encounter.ValidPrinces.Num() > 0 && !Encounter.ValidPrinces.Contains(Prince))
        {
            continue;
        }

        Result.Add(Encounter);
    }

    return Result;
}

FEncounterDefinition UEDEncounterSubsystem::SelectWeightedEncounter(const TArray<FEncounterDefinition>& Pool)
{
    float TotalWeight = 0.0f;
    for (const FEncounterDefinition& Encounter : Pool)
    {
        TotalWeight += FMath::Max(Encounter.Weight, 0.01f);
    }

    float Roll = RandomStream.FRandRange(0.0f, TotalWeight);
    float Accumulated = 0.0f;

    for (const FEncounterDefinition& Encounter : Pool)
    {
        Accumulated += FMath::Max(Encounter.Weight, 0.01f);
        if (Roll <= Accumulated)
        {
            return Encounter;
        }
    }

    return Pool.Last();
}
