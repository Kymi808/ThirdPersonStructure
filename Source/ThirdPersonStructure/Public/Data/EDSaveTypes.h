#pragma once

#include "CoreMinimal.h"
#include "Data/EDTypes.h"
#include "EDSaveTypes.generated.h"

USTRUCT(BlueprintType)
struct FInventorySaveData
{
    GENERATED_BODY()

    UPROPERTY()
    FName ItemId;

    UPROPERTY()
    int32 Quantity = 0;
};

USTRUCT(BlueprintType)
struct FFactionSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    EFactionId Faction;

    UPROPERTY()
    float Standing = 0.0f;
};

USTRUCT(BlueprintType)
struct FCampaignSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    FString SaveSlotName;

    UPROPERTY()
    EPrinceId SelectedPrince;

    UPROPERTY()
    int32 CurrentChapter = 0;

    UPROPERTY()
    ERoutePhase CurrentPhase;

    UPROPERTY()
    float Sanity = 100.0f;

    UPROPERTY()
    float Corruption = 0.0f;

    UPROPERTY()
    int32 SkillPoints = 0;

    UPROPERTY()
    int32 PlayerLevel = 1;

    UPROPERTY()
    int32 ExperiencePoints = 0;

    UPROPERTY()
    TArray<FName> UnlockedSkills;

    UPROPERTY()
    TArray<FName> CompletedEncounters;

    UPROPERTY()
    TArray<FName> ActiveEventFlags;

    UPROPERTY()
    TArray<FFactionSaveData> FactionStandings;

    UPROPERTY()
    TArray<FInventorySaveData> InventoryItems;

    UPROPERTY()
    FDateTime SaveTimestamp;
};
