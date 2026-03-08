#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/EDSaveTypes.h"
#include "EDSaveSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameSaved, const FString&, SlotName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameLoaded, const FString&, SlotName);

UCLASS()
class ENDSDAWNRPG_API UEDSaveSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Save")
    FOnGameSaved OnGameSaved;

    UPROPERTY(BlueprintAssignable, Category = "Save")
    FOnGameLoaded OnGameLoaded;

    UFUNCTION(BlueprintCallable, Category = "Save")
    bool SaveGame(const FString& SlotName);

    UFUNCTION(BlueprintCallable, Category = "Save")
    bool LoadGame(const FString& SlotName);

    UFUNCTION(BlueprintCallable, Category = "Save")
    bool DeleteSave(const FString& SlotName);

    UFUNCTION(BlueprintPure, Category = "Save")
    bool DoesSaveExist(const FString& SlotName) const;

    UFUNCTION(BlueprintPure, Category = "Save")
    TArray<FString> GetAllSaveSlots() const;

    UFUNCTION(BlueprintPure, Category = "Save")
    FString GetSaveDirectory() const;

protected:
    FString GetSaveFilePath(const FString& SlotName) const;

    TSharedPtr<FJsonObject> SerializeCampaignState() const;
    bool DeserializeCampaignState(const TSharedPtr<FJsonObject>& JsonObject);

    TSharedPtr<FJsonObject> SerializeFactionStandings() const;
    bool DeserializeFactionStandings(const TSharedPtr<FJsonObject>& JsonObject);
};
