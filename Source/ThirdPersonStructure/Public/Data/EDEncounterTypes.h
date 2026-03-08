#pragma once

#include "CoreMinimal.h"
#include "Data/EDTypes.h"
#include "EDEncounterTypes.generated.h"

UENUM(BlueprintType)
enum class EEncounterDifficulty : uint8
{
    Trivial,
    Easy,
    Normal,
    Hard,
    Deadly
};

USTRUCT(BlueprintType)
struct FFactionImpact
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EFactionId Faction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float StandingChange = 0.0f;
};

USTRUCT(BlueprintType)
struct FEncounterReward
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ExperiencePoints = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 SkillPoints = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float SanityChange = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CorruptionChange = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FFactionImpact> FactionImpacts;
};

USTRUCT(BlueprintType)
struct FEncounterChoice
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName ChoiceId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText ChoiceText;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText OutcomeDescription;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FEncounterReward Reward;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FName> RequiredSkills;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MinSanityRequired = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MaxCorruptionAllowed = 100.0f;
};

USTRUCT(BlueprintType)
struct FEncounterDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName EncounterId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Title;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText NarrativeText;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EEncounterType EncounterType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EEncounterDifficulty Difficulty;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ERoutePhase RequiredPhase;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<EPrinceId> ValidPrinces;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FEncounterChoice> Choices;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MinChapter = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 MaxChapter = 99;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Weight = 1.0f;
};
