// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Framework/Application/SlateApplication.h"
#include "Templates/SharedPointer.h"
#include "HAL/FileManagerGeneric.h"



int32 UMyBlueprintFunctionLibrary::GetLastMaxPlayerIndex()
{
    return UMyGameInstance::MAX_CONTROLLERS - 1;
}

void UMyBlueprintFunctionLibrary::SetActiveControllerID(const UObject* WorldContextObject, int32 ControllerID)
{
    UMyGameInstance* GameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (GameInstanceRef)
    {
        GameInstanceRef->SetActiveControllerID(ControllerID);
    }
}

int32 UMyBlueprintFunctionLibrary::GetActiveControllerID(const UObject* WorldContextObject)
{
    UMyGameInstance* GameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    if (GameInstanceRef)
    {
        return GameInstanceRef->GetActiveControllerID();
    }
    return -1;
}



void UMyBlueprintFunctionLibrary::RemoveAllPlayers(const UObject* WorldContextObject)
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(WorldContextObject, APlayerController::StaticClass(), FoundActors);
    for (AActor* Actor : FoundActors)
    {
        APlayerController* PC = Cast<APlayerController>(Actor);
        if (PC)
        {
            UGameplayStatics::RemovePlayer(PC, true);
        }
    }
}




APlayerController* UMyBlueprintFunctionLibrary::GetActivePlayerController(const UObject* WorldContextObject)
{
    return UGameplayStatics::GetPlayerControllerFromID(WorldContextObject, GetActiveControllerID(WorldContextObject));

}

ACharacter* UMyBlueprintFunctionLibrary::GetActivePlayerCharacter(const UObject* WorldContextObject)
{
    return GetActivePlayerController(WorldContextObject)->GetCharacter();
}

void UMyBlueprintFunctionLibrary::SetupActiveControllerID(const UObject* WorldContextObject, FKey Key, EStatusReturnType& Status)
{
    if (Key.IsGamepadKey())
    {
        for (int32 ControllerIndex = 0; ControllerIndex < UMyGameInstance::MAX_CONTROLLERS; ++ControllerIndex)
        {
            APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, ControllerIndex);
            if (PC->WasInputKeyJustPressed(Key))
            {
                SetActiveControllerID(WorldContextObject, ControllerIndex);
                RemoveAllPlayers(WorldContextObject);
                Status = EStatusReturnType::Success;
                return;
            }
        }
    }
    Status = EStatusReturnType::Fail;
}


FString UMyBlueprintFunctionLibrary::GetLevelName(FString ChapterName, FString LevelName)
{
    // will work as long level is on Gooie_Levels and the name is unique

    TArray<FString> MapNames = GetLevelsOnChapter(ChapterName);

    for (int i = 0; i < MapNames.Num(); i++) {

        if (MapNames[i].Contains(LevelName))
        {
            return MapNames[i];
        }
    }

    if (!MapNames[0].IsEmpty())
    {
        return MapNames[0];
    }
    else
    {
        return GetLevelsOnChapter("Chapter1")[0];
    }
}

TArray <FString> UMyBlueprintFunctionLibrary::GetLevelsOnChapter(FString ChapterName)
{
    TArray<FString>  MapNames;
    FString name;

    if (ChapterName == "Chapter2")
    {
        for (int32 i = 1; i < 6; i++)
        {
            name = (" C2-Level" + FString::FromInt(i));
            MapNames.Add(name);
            
        }
    }
    else if (ChapterName == "Chapter3")
    {
        for (int i = 1; i < 6; i++)
        {
            name = (" C3-Level"  + FString::FromInt(i));
            MapNames.Add(name);
        }
    }
    else 
    {
        for (int i = 1; i < 6; i++)
        {
            name = (" Level" + FString::FromInt(i));
            if (i == 4)
            {
                MapNames.Add(name + "_Final");
            }
            else if (i == 5)
            {
                MapNames.Add(name + "-ChaperEnd");
            }
            else
            {
                MapNames.Add(name + "-Final");
            }
        }

    }

  
    //IFileManager::Get().FindFilesRecursive(MapNames, *(FPaths::ProjectContentDir() + TEXT("Gooie_Levels/"+ ChapterName)), TEXT("*.umap"), true, false, false);

    return MapNames;
}

bool UMyBlueprintFunctionLibrary::DoesChapterExist(int chapter)
{
    TArray<FString>  ChaptersNames = GetAllChapters();
    //IFileManager::Get().FindFilesRecursive(ChaptersNames, *(FPaths::ProjectContentDir() + TEXT("Gooie_Levels/")), TEXT("*"), true, false, false);

    for (int i = 0; i < ChaptersNames.Num(); i++) {
        // Construct the substring to search for
        FString SubstringToFind = FString::Printf(TEXT("Chapter%d"), chapter);

        if (ChaptersNames[i].Contains(SubstringToFind))
        {
            return true;
        }
    }


    return false;
    
}

TArray<FString> UMyBlueprintFunctionLibrary::GetAllChapters()
{
    return {"Chapter1, Chapter2, Chapter3"};
}

FText UMyBlueprintFunctionLibrary::IntToFText(int32 Num)
{
    FString FormattedNum = FString::Printf(TEXT("%03d"), Num);
    return FText::FromString(FormattedNum);
}









