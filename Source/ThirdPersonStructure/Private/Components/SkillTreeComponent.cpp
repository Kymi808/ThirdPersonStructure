#include "Components/SkillTreeComponent.h"
#include "Data/SkillTreeDefinition.h"

USkillTreeComponent::USkillTreeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USkillTreeComponent::BeginPlay()
{
    Super::BeginPlay();
}

void USkillTreeComponent::InitializeSkillTree(USkillTreeDefinition* Definition)
{
    ActiveSkillTree = Definition;
    UnlockedSkills.Empty();
}

bool USkillTreeComponent::TryUnlockSkill(FName SkillId)
{
    if (!CanUnlockSkill(SkillId))
    {
        return false;
    }

    if (!ActiveSkillTree)
    {
        return false;
    }

    const FSkillDefinition* Skill = ActiveSkillTree->FindSkill(SkillId);
    if (!Skill)
    {
        return false;
    }

    SkillPoints -= Skill->PointCost;
    UnlockedSkills.Add(SkillId);

    OnSkillUnlocked.Broadcast(SkillId);
    OnSkillPointsChanged.Broadcast(SkillPoints, -Skill->PointCost);

    return true;
}

bool USkillTreeComponent::IsSkillUnlocked(FName SkillId) const
{
    return UnlockedSkills.Contains(SkillId);
}

bool USkillTreeComponent::CanUnlockSkill(FName SkillId) const
{
    if (!ActiveSkillTree || IsSkillUnlocked(SkillId))
    {
        return false;
    }

    const FSkillDefinition* Skill = ActiveSkillTree->FindSkill(SkillId);
    if (!Skill)
    {
        return false;
    }

    if (SkillPoints < Skill->PointCost)
    {
        return false;
    }

    return ArePrerequisitesMet(*Skill);
}

void USkillTreeComponent::AddSkillPoints(int32 Points)
{
    SkillPoints += Points;
    OnSkillPointsChanged.Broadcast(SkillPoints, Points);
}

void USkillTreeComponent::SetSkillPoints(int32 Points)
{
    const int32 Delta = Points - SkillPoints;
    SkillPoints = Points;
    if (Delta != 0)
    {
        OnSkillPointsChanged.Broadcast(SkillPoints, Delta);
    }
}

void USkillTreeComponent::ForceUnlockSkill(FName SkillId)
{
    if (!IsSkillUnlocked(SkillId))
    {
        UnlockedSkills.Add(SkillId);
        OnSkillUnlocked.Broadcast(SkillId);
    }
}

TArray<FSkillDefinition> USkillTreeComponent::GetAvailableSkills() const
{
    TArray<FSkillDefinition> Available;
    if (!ActiveSkillTree)
    {
        return Available;
    }

    for (const FSkillDefinition& Skill : ActiveSkillTree->Skills)
    {
        if (!IsSkillUnlocked(Skill.SkillId) && ArePrerequisitesMet(Skill) && SkillPoints >= Skill.PointCost)
        {
            Available.Add(Skill);
        }
    }
    return Available;
}

bool USkillTreeComponent::ArePrerequisitesMet(const FSkillDefinition& Skill) const
{
    for (const FName& PrereqId : Skill.Prerequisites)
    {
        if (!IsSkillUnlocked(PrereqId))
        {
            return false;
        }
    }
    return true;
}
