#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/EDSkillTypes.h"
#include "SkillTreeComponent.generated.h"

class USkillTreeDefinition;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillUnlocked, FName, SkillId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillPointsChanged, int32, NewPoints, int32, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ENDSDAWNRPG_API USkillTreeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USkillTreeComponent();

    UPROPERTY(BlueprintAssignable, Category = "Skill Tree")
    FOnSkillUnlocked OnSkillUnlocked;

    UPROPERTY(BlueprintAssignable, Category = "Skill Tree")
    FOnSkillPointsChanged OnSkillPointsChanged;

    UFUNCTION(BlueprintCallable, Category = "Skill Tree")
    void InitializeSkillTree(USkillTreeDefinition* Definition);

    UFUNCTION(BlueprintCallable, Category = "Skill Tree")
    bool TryUnlockSkill(FName SkillId);

    UFUNCTION(BlueprintPure, Category = "Skill Tree")
    bool IsSkillUnlocked(FName SkillId) const;

    UFUNCTION(BlueprintPure, Category = "Skill Tree")
    bool CanUnlockSkill(FName SkillId) const;

    UFUNCTION(BlueprintCallable, Category = "Skill Tree")
    void AddSkillPoints(int32 Points);

    UFUNCTION(BlueprintPure, Category = "Skill Tree")
    FORCEINLINE int32 GetSkillPoints() const { return SkillPoints; }

    UFUNCTION(BlueprintPure, Category = "Skill Tree")
    FORCEINLINE TArray<FName> GetUnlockedSkills() const { return UnlockedSkills; }

    UFUNCTION(BlueprintPure, Category = "Skill Tree")
    TArray<FSkillDefinition> GetAvailableSkills() const;

    UFUNCTION(BlueprintPure, Category = "Skill Tree")
    const USkillTreeDefinition* GetSkillTreeDefinition() const { return ActiveSkillTree; }

    void SetSkillPoints(int32 Points);
    void ForceUnlockSkill(FName SkillId);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Tree")
    int32 SkillPoints = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Tree")
    TArray<FName> UnlockedSkills;

    UPROPERTY()
    USkillTreeDefinition* ActiveSkillTree;

    bool ArePrerequisitesMet(const FSkillDefinition& Skill) const;
};
