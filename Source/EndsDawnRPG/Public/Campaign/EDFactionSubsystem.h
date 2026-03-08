#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/EDTypes.h"
#include "EDFactionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnFactionStandingChanged, EFactionId, Faction, float, NewStanding, float, Delta);

UCLASS()
class ENDSDAWNRPG_API UEDFactionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UPROPERTY(BlueprintAssignable, Category = "Factions")
    FOnFactionStandingChanged OnFactionStandingChanged;

    UFUNCTION(BlueprintCallable, Category = "Factions")
    void InitializeFactionStandings();

    UFUNCTION(BlueprintCallable, Category = "Factions")
    void ModifyFactionStanding(EFactionId Faction, float Delta);

    UFUNCTION(BlueprintCallable, Category = "Factions")
    void SetFactionStanding(EFactionId Faction, float NewStanding);

    UFUNCTION(BlueprintPure, Category = "Factions")
    float GetFactionStanding(EFactionId Faction) const;

    UFUNCTION(BlueprintPure, Category = "Factions")
    EFactionId GetHighestStandingFaction() const;

    UFUNCTION(BlueprintPure, Category = "Factions")
    EFactionId GetLowestStandingFaction() const;

    UFUNCTION(BlueprintPure, Category = "Factions")
    TMap<EFactionId, float> GetAllStandings() const { return FactionStandings; }

    UFUNCTION(BlueprintPure, Category = "Factions")
    bool IsFactionHostile(EFactionId Faction) const;

    UFUNCTION(BlueprintPure, Category = "Factions")
    bool IsFactionAllied(EFactionId Faction) const;

protected:
    UPROPERTY()
    TMap<EFactionId, float> FactionStandings;

    float MinStanding = -100.0f;
    float MaxStanding = 100.0f;
    float HostileThreshold = -50.0f;
    float AlliedThreshold = 50.0f;
};
