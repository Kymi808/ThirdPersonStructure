#pragma once

#include "CoreMinimal.h"
#include "Data/EDTypes.h"
#include "EDSkillTypes.generated.h"

UENUM(BlueprintType)
enum class ESkillCategory : uint8
{
    Combat,
    Political,
    Magic,
    Stealth,
    Leadership
};

USTRUCT(BlueprintType)
struct FSkillDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName SkillId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ESkillCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 PointCost = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 RequiredLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FName> Prerequisites;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float EffectMagnitude = 1.0f;
};
