#include "Campaign/EDFactionSubsystem.h"

void UEDFactionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    InitializeFactionStandings();
}

void UEDFactionSubsystem::InitializeFactionStandings()
{
    FactionStandings.Empty();
    FactionStandings.Add(EFactionId::CentralNobles, 0.0f);
    FactionStandings.Add(EFactionId::WesternHouses, 0.0f);
    FactionStandings.Add(EFactionId::NorthernMilitary, 0.0f);
    FactionStandings.Add(EFactionId::SouthernFaction, 0.0f);
    FactionStandings.Add(EFactionId::MagicAssociation, 0.0f);
    FactionStandings.Add(EFactionId::HolyNation, 0.0f);
}

void UEDFactionSubsystem::ModifyFactionStanding(EFactionId Faction, float Delta)
{
    float* Current = FactionStandings.Find(Faction);
    if (!Current)
    {
        return;
    }

    const float OldStanding = *Current;
    *Current = FMath::Clamp(*Current + Delta, MinStanding, MaxStanding);
    const float ActualDelta = *Current - OldStanding;

    if (!FMath::IsNearlyZero(ActualDelta))
    {
        OnFactionStandingChanged.Broadcast(Faction, *Current, ActualDelta);
    }
}

void UEDFactionSubsystem::SetFactionStanding(EFactionId Faction, float NewStanding)
{
    float ClampedStanding = FMath::Clamp(NewStanding, MinStanding, MaxStanding);
    float* Current = FactionStandings.Find(Faction);
    if (Current)
    {
        float Delta = ClampedStanding - *Current;
        *Current = ClampedStanding;
        if (!FMath::IsNearlyZero(Delta))
        {
            OnFactionStandingChanged.Broadcast(Faction, ClampedStanding, Delta);
        }
    }
}

float UEDFactionSubsystem::GetFactionStanding(EFactionId Faction) const
{
    const float* Standing = FactionStandings.Find(Faction);
    return Standing ? *Standing : 0.0f;
}

EFactionId UEDFactionSubsystem::GetHighestStandingFaction() const
{
    EFactionId Highest = EFactionId::CentralNobles;
    float HighestValue = -101.0f;

    for (const auto& Pair : FactionStandings)
    {
        if (Pair.Value > HighestValue)
        {
            HighestValue = Pair.Value;
            Highest = Pair.Key;
        }
    }
    return Highest;
}

EFactionId UEDFactionSubsystem::GetLowestStandingFaction() const
{
    EFactionId Lowest = EFactionId::CentralNobles;
    float LowestValue = 101.0f;

    for (const auto& Pair : FactionStandings)
    {
        if (Pair.Value < LowestValue)
        {
            LowestValue = Pair.Value;
            Lowest = Pair.Key;
        }
    }
    return Lowest;
}

bool UEDFactionSubsystem::IsFactionHostile(EFactionId Faction) const
{
    return GetFactionStanding(Faction) <= HostileThreshold;
}

bool UEDFactionSubsystem::IsFactionAllied(EFactionId Faction) const
{
    return GetFactionStanding(Faction) >= AlliedThreshold;
}
