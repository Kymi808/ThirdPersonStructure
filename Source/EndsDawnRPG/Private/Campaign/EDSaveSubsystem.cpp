#include "Campaign/EDSaveSubsystem.h"
#include "Campaign/EDCampaignStateSubsystem.h"
#include "Campaign/EDFactionSubsystem.h"
#include "Campaign/EDEncounterSubsystem.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"

bool UEDSaveSubsystem::SaveGame(const FString& SlotName)
{
    const UGameInstance* GI = GetGameInstance();
    if (!GI)
    {
        return false;
    }

    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject());

    RootObject->SetStringField(TEXT("SaveSlotName"), SlotName);
    RootObject->SetStringField(TEXT("SaveTimestamp"), FDateTime::Now().ToString());
    RootObject->SetNumberField(TEXT("SaveVersion"), 1);

    // Campaign state
    TSharedPtr<FJsonObject> CampaignJson = SerializeCampaignState();
    if (CampaignJson.IsValid())
    {
        RootObject->SetObjectField(TEXT("CampaignState"), CampaignJson);
    }

    // Faction standings
    TSharedPtr<FJsonObject> FactionJson = SerializeFactionStandings();
    if (FactionJson.IsValid())
    {
        RootObject->SetObjectField(TEXT("FactionStandings"), FactionJson);
    }

    // Encounter history
    const UEDEncounterSubsystem* EncounterSub = GI->GetSubsystem<UEDEncounterSubsystem>();
    if (EncounterSub)
    {
        TArray<TSharedPtr<FJsonValue>> EncounterArray;
        for (const FName& Id : EncounterSub->GetCompletedEncounters())
        {
            EncounterArray.Add(MakeShareable(new FJsonValueString(Id.ToString())));
        }
        RootObject->SetArrayField(TEXT("CompletedEncounters"), EncounterArray);
    }

    // Serialize to string
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    if (!FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer))
    {
        return false;
    }

    // Write to file
    const FString FilePath = GetSaveFilePath(SlotName);
    if (!FFileHelper::SaveStringToFile(OutputString, *FilePath))
    {
        return false;
    }

    OnGameSaved.Broadcast(SlotName);
    return true;
}

bool UEDSaveSubsystem::LoadGame(const FString& SlotName)
{
    const FString FilePath = GetSaveFilePath(SlotName);

    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        return false;
    }

    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
    {
        return false;
    }

    // Campaign state
    const TSharedPtr<FJsonObject>* CampaignJson = nullptr;
    if (RootObject->TryGetObjectField(TEXT("CampaignState"), CampaignJson))
    {
        DeserializeCampaignState(*CampaignJson);
    }

    // Faction standings
    const TSharedPtr<FJsonObject>* FactionJson = nullptr;
    if (RootObject->TryGetObjectField(TEXT("FactionStandings"), FactionJson))
    {
        DeserializeFactionStandings(*FactionJson);
    }

    // Encounter history
    UGameInstance* GI = GetGameInstance();
    if (GI)
    {
        UEDEncounterSubsystem* EncounterSub = GI->GetSubsystem<UEDEncounterSubsystem>();
        if (EncounterSub)
        {
            const TArray<TSharedPtr<FJsonValue>>* EncounterArray = nullptr;
            if (RootObject->TryGetArrayField(TEXT("CompletedEncounters"), EncounterArray))
            {
                TArray<FName> CompletedEncounters;
                for (const TSharedPtr<FJsonValue>& Val : *EncounterArray)
                {
                    CompletedEncounters.Add(FName(*Val->AsString()));
                }
                EncounterSub->SetCompletedEncounters(CompletedEncounters);
            }
        }
    }

    OnGameLoaded.Broadcast(SlotName);
    return true;
}

bool UEDSaveSubsystem::DeleteSave(const FString& SlotName)
{
    const FString FilePath = GetSaveFilePath(SlotName);
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    return PlatformFile.DeleteFile(*FilePath);
}

bool UEDSaveSubsystem::DoesSaveExist(const FString& SlotName) const
{
    const FString FilePath = GetSaveFilePath(SlotName);
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    return PlatformFile.FileExists(*FilePath);
}

TArray<FString> UEDSaveSubsystem::GetAllSaveSlots() const
{
    TArray<FString> SaveSlots;
    const FString SaveDir = GetSaveDirectory();
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    TArray<FString> FoundFiles;
    PlatformFile.FindFiles(FoundFiles, *SaveDir, TEXT(".json"));

    for (const FString& File : FoundFiles)
    {
        SaveSlots.Add(FPaths::GetBaseFilename(File));
    }

    return SaveSlots;
}

FString UEDSaveSubsystem::GetSaveDirectory() const
{
    return FPaths::ProjectSavedDir() / TEXT("SaveGames");
}

FString UEDSaveSubsystem::GetSaveFilePath(const FString& SlotName) const
{
    return GetSaveDirectory() / (SlotName + TEXT(".json"));
}

TSharedPtr<FJsonObject> UEDSaveSubsystem::SerializeCampaignState() const
{
    const UGameInstance* GI = GetGameInstance();
    if (!GI)
    {
        return nullptr;
    }

    const UEDCampaignStateSubsystem* CampaignState = GI->GetSubsystem<UEDCampaignStateSubsystem>();
    if (!CampaignState)
    {
        return nullptr;
    }

    TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());
    Obj->SetNumberField(TEXT("CurrentChapter"), CampaignState->CurrentChapter);
    Obj->SetNumberField(TEXT("SelectedPrince"), static_cast<int32>(CampaignState->SelectedPrince));
    Obj->SetNumberField(TEXT("CurrentPhase"), static_cast<int32>(CampaignState->CurrentPhase));
    Obj->SetNumberField(TEXT("Sanity"), CampaignState->Sanity);
    Obj->SetNumberField(TEXT("Corruption"), CampaignState->Corruption);
    Obj->SetNumberField(TEXT("SkillPoints"), CampaignState->SkillPoints);
    Obj->SetNumberField(TEXT("PlayerLevel"), CampaignState->PlayerLevel);
    Obj->SetNumberField(TEXT("ExperiencePoints"), CampaignState->ExperiencePoints);

    // Event flags
    TArray<TSharedPtr<FJsonValue>> FlagArray;
    for (const FName& Flag : CampaignState->ActiveEventFlags)
    {
        FlagArray.Add(MakeShareable(new FJsonValueString(Flag.ToString())));
    }
    Obj->SetArrayField(TEXT("ActiveEventFlags"), FlagArray);

    // Unlocked skills
    TArray<TSharedPtr<FJsonValue>> SkillArray;
    for (const FName& Skill : CampaignState->UnlockedSkills)
    {
        SkillArray.Add(MakeShareable(new FJsonValueString(Skill.ToString())));
    }
    Obj->SetArrayField(TEXT("UnlockedSkills"), SkillArray);

    return Obj;
}

bool UEDSaveSubsystem::DeserializeCampaignState(const TSharedPtr<FJsonObject>& JsonObject)
{
    UGameInstance* GI = GetGameInstance();
    if (!GI || !JsonObject.IsValid())
    {
        return false;
    }

    UEDCampaignStateSubsystem* CampaignState = GI->GetSubsystem<UEDCampaignStateSubsystem>();
    if (!CampaignState)
    {
        return false;
    }

    CampaignState->CurrentChapter = JsonObject->GetIntegerField(TEXT("CurrentChapter"));
    CampaignState->SelectedPrince = static_cast<EPrinceId>(JsonObject->GetIntegerField(TEXT("SelectedPrince")));
    CampaignState->CurrentPhase = static_cast<ERoutePhase>(JsonObject->GetIntegerField(TEXT("CurrentPhase")));
    CampaignState->Sanity = JsonObject->GetNumberField(TEXT("Sanity"));
    CampaignState->Corruption = JsonObject->GetNumberField(TEXT("Corruption"));
    CampaignState->SkillPoints = JsonObject->GetIntegerField(TEXT("SkillPoints"));
    CampaignState->PlayerLevel = JsonObject->GetIntegerField(TEXT("PlayerLevel"));
    CampaignState->ExperiencePoints = JsonObject->GetIntegerField(TEXT("ExperiencePoints"));

    // Event flags
    CampaignState->ActiveEventFlags.Empty();
    const TArray<TSharedPtr<FJsonValue>>* FlagArray = nullptr;
    if (JsonObject->TryGetArrayField(TEXT("ActiveEventFlags"), FlagArray))
    {
        for (const TSharedPtr<FJsonValue>& Val : *FlagArray)
        {
            CampaignState->ActiveEventFlags.Add(FName(*Val->AsString()));
        }
    }

    // Unlocked skills
    CampaignState->UnlockedSkills.Empty();
    const TArray<TSharedPtr<FJsonValue>>* SkillArray = nullptr;
    if (JsonObject->TryGetArrayField(TEXT("UnlockedSkills"), SkillArray))
    {
        for (const TSharedPtr<FJsonValue>& Val : *SkillArray)
        {
            CampaignState->UnlockedSkills.Add(FName(*Val->AsString()));
        }
    }

    return true;
}

TSharedPtr<FJsonObject> UEDSaveSubsystem::SerializeFactionStandings() const
{
    const UGameInstance* GI = GetGameInstance();
    if (!GI)
    {
        return nullptr;
    }

    const UEDFactionSubsystem* FactionSub = GI->GetSubsystem<UEDFactionSubsystem>();
    if (!FactionSub)
    {
        return nullptr;
    }

    TSharedPtr<FJsonObject> Obj = MakeShareable(new FJsonObject());

    const TMap<EFactionId, float>& Standings = FactionSub->GetAllStandings();
    for (const auto& Pair : Standings)
    {
        Obj->SetNumberField(FString::FromInt(static_cast<int32>(Pair.Key)), Pair.Value);
    }

    return Obj;
}

bool UEDSaveSubsystem::DeserializeFactionStandings(const TSharedPtr<FJsonObject>& JsonObject)
{
    UGameInstance* GI = GetGameInstance();
    if (!GI || !JsonObject.IsValid())
    {
        return false;
    }

    UEDFactionSubsystem* FactionSub = GI->GetSubsystem<UEDFactionSubsystem>();
    if (!FactionSub)
    {
        return false;
    }

    FactionSub->InitializeFactionStandings();

    for (const auto& Pair : JsonObject->Values)
    {
        int32 FactionIndex = FCString::Atoi(*Pair.Key);
        float Standing = Pair.Value->AsNumber();
        FactionSub->SetFactionStanding(static_cast<EFactionId>(FactionIndex), Standing);
    }

    return true;
}
