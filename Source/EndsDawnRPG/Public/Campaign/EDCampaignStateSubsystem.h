#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/EDTypes.h"
#include "EDCampaignStateSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChapterChanged, int32, NewChapter, int32, OldChapter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPhaseChanged, ERoutePhase, NewPhase, ERoutePhase, OldPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPrinceSelected, EPrinceId, Prince);

UCLASS()
class ENDSDAWNRPG_API UEDCampaignStateSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UPROPERTY(BlueprintAssignable, Category = "Campaign")
    FOnChapterChanged OnChapterChanged;

    UPROPERTY(BlueprintAssignable, Category = "Campaign")
    FOnPhaseChanged OnPhaseChanged;

    UPROPERTY(BlueprintAssignable, Category = "Campaign")
    FOnPrinceSelected OnPrinceSelected;

    // Core campaign state
    UPROPERTY(BlueprintReadWrite, Category = "Campaign")
    int32 CurrentChapter = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Campaign")
    EPrinceId SelectedPrince;

    UPROPERTY(BlueprintReadWrite, Category = "Campaign")
    ERoutePhase CurrentPhase;

    // Player stats (mirrored for save/load)
    UPROPERTY(BlueprintReadWrite, Category = "Campaign|Stats")
    float Sanity = 100.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Campaign|Stats")
    float Corruption = 0.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Campaign|Stats")
    int32 SkillPoints = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Campaign|Stats")
    int32 PlayerLevel = 1;

    UPROPERTY(BlueprintReadWrite, Category = "Campaign|Stats")
    int32 ExperiencePoints = 0;

    // Progression tracking
    UPROPERTY(BlueprintReadWrite, Category = "Campaign|Progression")
    TArray<FName> UnlockedSkills;

    UPROPERTY(BlueprintReadWrite, Category = "Campaign|Progression")
    TArray<FName> ActiveEventFlags;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void SelectPrince(EPrinceId Prince);

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void AdvanceChapter();

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void SetPhase(ERoutePhase NewPhase);

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void SetEventFlag(FName FlagName);

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void ClearEventFlag(FName FlagName);

    UFUNCTION(BlueprintPure, Category = "Campaign")
    bool HasEventFlag(FName FlagName) const;

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void AddExperience(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Campaign")
    void ResetCampaign();

protected:
    int32 CalculateLevelFromExperience() const;

    UPROPERTY(EditDefaultsOnly, Category = "Campaign|Leveling")
    int32 BaseExperiencePerLevel = 100;

    UPROPERTY(EditDefaultsOnly, Category = "Campaign|Leveling")
    float ExperienceScalingFactor = 1.5f;
};
