#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/EDTypes.h"
#include "Data/EDSkillTypes.h"
#include "SkillTreeDefinition.generated.h"

UCLASS(BlueprintType)
class THIRDPERSONSTRUCTURE_API USkillTreeDefinition : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree")
    FName SkillTreeId;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree")
    EPrinceId OwnerPrince;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree")
    TArray<FSkillDefinition> Skills;

    const FSkillDefinition* FindSkill(FName SkillId) const
    {
        return Skills.FindByPredicate([&SkillId](const FSkillDefinition& Skill)
        {
            return Skill.SkillId == SkillId;
        });
    }
};
