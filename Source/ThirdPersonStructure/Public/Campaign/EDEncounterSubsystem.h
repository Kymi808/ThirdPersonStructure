#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/EDEncounterTypes.h"
#include "EDEncounterSubsystem.generated.h"

class UEncounterDataAsset;
class UEDCampaignStateSubsystem;
class UEDFactionSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEncounterGenerated, const FEncounterDefinition&, Encounter);

UCLASS()
class THIRDPERSONSTRUCTURE_API UEDEncounterSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UPROPERTY(BlueprintAssignable, Category = "Encounters")
    FOnEncounterGenerated OnEncounterGenerated;

    UFUNCTION(BlueprintCallable, Category = "Encounters")
    void SetEncounterDataAsset(UEncounterDataAsset* DataAsset);

    UFUNCTION(BlueprintCallable, Category = "Encounters")
    FEncounterDefinition GenerateEncounter();

    UFUNCTION(BlueprintCallable, Category = "Encounters")
    TArray<FEncounterDefinition> GenerateEncounterPool(int32 Count);

    UFUNCTION(BlueprintCallable, Category = "Encounters")
    void MarkEncounterCompleted(FName EncounterId);

    UFUNCTION(BlueprintPure, Category = "Encounters")
    bool HasCompletedEncounter(FName EncounterId) const;

    UFUNCTION(BlueprintPure, Category = "Encounters")
    TArray<FName> GetCompletedEncounters() const { return CompletedEncounters; }

    void SetCompletedEncounters(const TArray<FName>& Encounters);

protected:
    UPROPERTY()
    UEncounterDataAsset* ActiveEncounterData;

    UPROPERTY()
    TArray<FName> CompletedEncounters;

    FRandomStream RandomStream;

    TArray<FEncounterDefinition> FilterValidEncounters() const;
    FEncounterDefinition SelectWeightedEncounter(const TArray<FEncounterDefinition>& Pool);
};
