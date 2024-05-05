// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EStatusReturnType : uint8
{
	Success UMETA(DisplayName = "On Success"),
	Fail UMETA(DisplayName = "On Failure")
};

UCLASS()
class ESTRIX_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintPure, Category = "MyBlueprintFunctionLibrary")
		static int32 GetLastMaxPlayerIndex();



	UFUNCTION(BlueprintCallable, Category = "MyBlueprintFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
		static void SetActiveControllerID(const UObject* WorldContextObject, int32 ControllerID);

	UFUNCTION(BlueprintPure, Category = "MyBlueprintFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
		static int32 GetActiveControllerID(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "MyBlueprintFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
		static void RemoveAllPlayers(const UObject* WorldContextObject);



	UFUNCTION(BlueprintPure, Category = "MyBlueprintFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
		static class APlayerController* GetActivePlayerController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "MyBlueprintFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
		static class ACharacter* GetActivePlayerCharacter(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "MyBlueprintFunctionLibrary", meta = (WorldContext = "WorldContextObject", ExpandEnumAsExecs = "Status"))
		static void SetupActiveControllerID(const UObject* WorldContextObject, FKey Key, EStatusReturnType& Status);



	UFUNCTION(BlueprintPure, Category = "MyBlueprintFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
		static FString GetLevelName(FString ChapterName, FString LevelName);

	UFUNCTION(BlueprintPure, Category = "MyBlueprintFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
		static TArray<FString> GetLevelsOnChapter(FString ChapterName);
	
	UFUNCTION(BlueprintPure, Category = "MyBlueprintFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
		static bool DoesChapterExist(int chapter);

	UFUNCTION(BlueprintPure, Category = "MyBlueprintFunctionLibrary", meta = (WorldContext = "WorldContextObject"))
		static TArray<FString> GetAllChapters();

		static FText IntToFText(int32 Num);


};
