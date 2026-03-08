#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/EDEncounterTypes.h"
#include "EncounterDataAsset.generated.h"

UCLASS(BlueprintType)
class ENDSDAWNRPG_API UEncounterDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounters")
    TArray<FEncounterDefinition> Encounters;

    TArray<FEncounterDefinition> GetEncountersForPhase(ERoutePhase Phase) const
    {
        TArray<FEncounterDefinition> Result;
        for (const FEncounterDefinition& Encounter : Encounters)
        {
            if (Encounter.RequiredPhase == Phase)
            {
                Result.Add(Encounter);
            }
        }
        return Result;
    }

    TArray<FEncounterDefinition> GetEncountersForPrince(EPrinceId Prince) const
    {
        TArray<FEncounterDefinition> Result;
        for (const FEncounterDefinition& Encounter : Encounters)
        {
            if (Encounter.ValidPrinces.Num() == 0 || Encounter.ValidPrinces.Contains(Prince))
            {
                Result.Add(Encounter);
            }
        }
        return Result;
    }
};
