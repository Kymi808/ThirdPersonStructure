#include "Components/SanityCorruptionComponent.h"

USanityCorruptionComponent::USanityCorruptionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USanityCorruptionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void USanityCorruptionComponent::ModifySanity(float Delta)
{
    const float OldSanity = Sanity;
    Sanity = FMath::Clamp(Sanity + Delta, 0.0f, MaxSanity);
    const float ActualDelta = Sanity - OldSanity;

    if (!FMath::IsNearlyZero(ActualDelta))
    {
        OnSanityChanged.Broadcast(Sanity, ActualDelta);

        if (Sanity <= 0.0f)
        {
            OnSanityDepleted.Broadcast();
        }
    }
}

void USanityCorruptionComponent::ModifyCorruption(float Delta)
{
    const float OldCorruption = Corruption;
    Corruption = FMath::Clamp(Corruption + Delta, 0.0f, MaxCorruption);
    const float ActualDelta = Corruption - OldCorruption;

    if (!FMath::IsNearlyZero(ActualDelta))
    {
        OnCorruptionChanged.Broadcast(Corruption, ActualDelta);

        if (Corruption >= MaxCorruption)
        {
            OnFullyCorrupted.Broadcast();
        }
    }
}

void USanityCorruptionComponent::SetSanity(float NewValue)
{
    const float Delta = NewValue - Sanity;
    Sanity = FMath::Clamp(NewValue, 0.0f, MaxSanity);

    if (!FMath::IsNearlyZero(Delta))
    {
        OnSanityChanged.Broadcast(Sanity, Delta);
    }
}

void USanityCorruptionComponent::SetCorruption(float NewValue)
{
    const float Delta = NewValue - Corruption;
    Corruption = FMath::Clamp(NewValue, 0.0f, MaxCorruption);

    if (!FMath::IsNearlyZero(Delta))
    {
        OnCorruptionChanged.Broadcast(Corruption, Delta);
    }
}

bool USanityCorruptionComponent::IsSanityLow() const
{
    return Sanity <= LowSanityThreshold;
}

bool USanityCorruptionComponent::IsCorruptionHigh() const
{
    return Corruption >= HighCorruptionThreshold;
}

float USanityCorruptionComponent::GetSanityPercent() const
{
    return MaxSanity > 0.0f ? Sanity / MaxSanity : 0.0f;
}

float USanityCorruptionComponent::GetCorruptionPercent() const
{
    return MaxCorruption > 0.0f ? Corruption / MaxCorruption : 0.0f;
}
