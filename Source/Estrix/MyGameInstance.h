// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ControllerDisconnectedWidget.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */

class ULoadingWidget;

UCLASS()
class ESTRIX_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

	FTSTicker::FDelegateHandle TickDelegateHandle;
	int32 ActiveControllerId = -1;

	bool bDidContollerDisconnectPauseGame = false;

	UPROPERTY()
		UControllerDisconnectedWidget* ControllerDisconnectedWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UControllerDisconnectedWidget> ControllerDisconnectedWidgetClass;


	UFUNCTION()
	void OnControllerChange(EInputDeviceConnectionState ConnectionState, FPlatformUserId userID, FInputDeviceId inputDeviceID);


	UPROPERTY()
	ULoadingWidget* LoadingWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<ULoadingWidget> LoadingWidgetClass;

public:
	static const int32 MAX_CONTROLLERS = 4;

	virtual void Init() override;
	virtual void Shutdown() override;

	bool Tick(float DeltaSeconds);

	UFUNCTION()
	void BeginLoadingScreen(const FString& InMapName);

	UFUNCTION()
	void EndLoadingScreen(UWorld* InLoadedWorld);

	void SetActiveControllerID(int32 ControllerID);
	int32 GetActiveControllerID();
	
public:

};
