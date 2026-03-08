#include "Campaign/EDCampaignStateSubsystem.h"

void UEDCampaignStateSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    ResetCampaign();
}

void UEDCampaignStateSubsystem::SelectPrince(EPrinceId Prince)
{
    SelectedPrince = Prince;
    OnPrinceSelected.Broadcast(Prince);
}

void UEDCampaignStateSubsystem::AdvanceChapter()
{
    const int32 OldChapter = CurrentChapter;
    CurrentChapter++;
    OnChapterChanged.Broadcast(CurrentChapter, OldChapter);
}

void UEDCampaignStateSubsystem::SetPhase(ERoutePhase NewPhase)
{
    const ERoutePhase OldPhase = CurrentPhase;
    if (NewPhase != OldPhase)
    {
        CurrentPhase = NewPhase;
        OnPhaseChanged.Broadcast(NewPhase, OldPhase);
    }
}

void UEDCampaignStateSubsystem::SetEventFlag(FName FlagName)
{
    ActiveEventFlags.AddUnique(FlagName);
}

void UEDCampaignStateSubsystem::ClearEventFlag(FName FlagName)
{
    ActiveEventFlags.Remove(FlagName);
}

bool UEDCampaignStateSubsystem::HasEventFlag(FName FlagName) const
{
    return ActiveEventFlags.Contains(FlagName);
}

void UEDCampaignStateSubsystem::AddExperience(int32 Amount)
{
    ExperiencePoints += Amount;
    const int32 NewLevel = CalculateLevelFromExperience();
    if (NewLevel > PlayerLevel)
    {
        PlayerLevel = NewLevel;
    }
}

void UEDCampaignStateSubsystem::ResetCampaign()
{
    CurrentChapter = 0;
    SelectedPrince = EPrinceId::Maximilian;
    CurrentPhase = ERoutePhase::Prologue;
    Sanity = 100.0f;
    Corruption = 0.0f;
    SkillPoints = 0;
    PlayerLevel = 1;
    ExperiencePoints = 0;
    UnlockedSkills.Empty();
    ActiveEventFlags.Empty();
}

int32 UEDCampaignStateSubsystem::CalculateLevelFromExperience() const
{
    int32 Level = 1;
    int32 ExpRequired = BaseExperiencePerLevel;
    int32 TotalExpUsed = 0;

    while (TotalExpUsed + ExpRequired <= ExperiencePoints)
    {
        TotalExpUsed += ExpRequired;
        Level++;
        ExpRequired = FMath::FloorToInt(BaseExperiencePerLevel * FMath::Pow(ExperienceScalingFactor, Level - 1));
    }

    return Level;
}
