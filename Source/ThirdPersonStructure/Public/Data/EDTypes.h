#pragma once

#include "CoreMinimal.h"
#include "EDTypes.generated.h"

UENUM(BlueprintType)
enum class EPrinceId : uint8
{
    Maximilian,
    Alaric,
    Wilhelm,
    Conrad,
    Rupert,
    Gerald,
    Casimir,
    Leonhardt
};

UENUM(BlueprintType)
enum class EFactionId : uint8
{
    CentralNobles,
    WesternHouses,
    NorthernMilitary,
    SouthernFaction,
    MagicAssociation,
    HolyNation
};

UENUM(BlueprintType)
enum class ERoutePhase : uint8
{
    Prologue,
    PoliticalManeuvering,
    KingsDeath,
    AurelvianInvasion,
    SuccessionWar,
    Epilogue
};

UENUM(BlueprintType)
enum class EEncounterType : uint8
{
    Combat,
    Political,
    Exploration,
    Social,
    Mystery
};
