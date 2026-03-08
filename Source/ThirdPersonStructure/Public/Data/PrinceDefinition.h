#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/EDTypes.h"
#include "PrinceDefinition.generated.h"

USTRUCT(BlueprintType)
struct FPrinceStartingFaction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EFactionId Faction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float StartingStanding = 0.0f;
};

UCLASS(BlueprintType)
class THIRDPERSONSTRUCTURE_API UPrinceDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prince")
    EPrinceId PrinceId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prince")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prince")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prince")
    FText Archetype;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prince")
    FText RouteTheme;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prince")
    bool bIsUnlockable = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prince|Starting Stats")
    TArray<FPrinceStartingFaction> StartingFactionStandings;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prince|Starting Stats")
    float StartingSanity = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prince|Starting Stats")
    float StartingCorruption = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prince|Assets")
    TSoftObjectPtr<UTexture2D> Portrait;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Prince|Skill Tree")
    FName SkillTreeId;
};
