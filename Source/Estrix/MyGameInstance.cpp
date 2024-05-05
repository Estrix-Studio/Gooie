// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "LoadingWidget.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include "Kismet/GameplayStatics.h"
#include "MyBlueprintFunctionLibrary.h"
#include "MoviePlayer.h"
#include "LoadingScreenModule.h"

void UMyGameInstance::Init()
{
	Super::Init();

	FTickerDelegate TickDelegate = FTickerDelegate::CreateUObject(this, &UMyGameInstance::Tick);
	TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickDelegate);
	//GetWorld()->Exec()
	IPlatformInputDeviceMapper::Get().GetOnInputDeviceConnectionChange().AddUObject(this, &UMyGameInstance::OnControllerChange);

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UMyGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMyGameInstance::EndLoadingScreen);
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();
	FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
}


bool UMyGameInstance::Tick(float DeltaTime)
{
	if (GEngine)
	{
		FString message = FString::Printf(TEXT("Active Controller ID: %d"), ActiveControllerId);

		GEngine->AddOnScreenDebugMessage(1, 0.2f, FColor::Green, *message);
	}
	if (APlayerController* ActivePC = UMyBlueprintFunctionLibrary::GetActivePlayerController(this))
	{
		LoadingWidget = CreateWidget<ULoadingWidget>(ActivePC, LoadingWidgetClass);
	}
	return true;
}


void UMyGameInstance::BeginLoadingScreen(const FString& InMapName)
{
	if (!IsRunningDedicatedServer())
	{

		if (InMapName.Contains("Screen"))
		{
			//no loading screen requarid
		}
		else
		{
			if (LoadingWidget != NULL)
			{
				FLoadingScreenAttributes LoadingScreen;
				LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
				LoadingScreen.MinimumLoadingScreenDisplayTime = 5.0f;
				TSharedPtr<SWidget> WidgetPtr = LoadingWidget->TakeWidget();
				LoadingScreen.WidgetLoadingScreen = WidgetPtr;
				GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
			}
			else
			{
				ILoadingScreenModule* LoadingScreenModule = FModuleManager::LoadModulePtr<ILoadingScreenModule>("LoadingScreenModule");
				if (LoadingScreenModule != NULL)
				{
					LoadingScreenModule->StartInGameLoadingScreen();
				}
			}

		}
	}

}

void UMyGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{

}


void UMyGameInstance::SetActiveControllerID(int32 ControllerID)
{
	ActiveControllerId = ControllerID;
}

int32 UMyGameInstance::GetActiveControllerID()
{
	return ActiveControllerId;
}

void UMyGameInstance::OnControllerChange(EInputDeviceConnectionState ConnectionState, FPlatformUserId userID, FInputDeviceId inputDeviceID)
{
	UE_LOG(LogTemp, Warning, TEXT("Controller Changed!"));

	FString UserId_Text;
	if (userID == PLATFORMUSERID_NONE)
	{
		UserId_Text = TEXT("None");
	}
	else
	{
		UserId_Text = FString::Printf(TEXT("%d"), userID.GetInternalId());
	}

	if (ConnectionState == EInputDeviceConnectionState::Disconnected)
	{
		FString message = FString::Printf(TEXT("Controller connection changed - UserIdx:%d, UserID:%s, Disconnected"), inputDeviceID.GetId(), *UserId_Text);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, message);
	}
	else if (ConnectionState == EInputDeviceConnectionState::Connected)
	{
		FString message = FString::Printf(TEXT("Controller connection changed - UserIdx:%d, UserID:%s, Connected"), inputDeviceID.GetId(), *UserId_Text);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, message);
	}

	if (GetActiveControllerID() == inputDeviceID.GetId())
	{
		if (APlayerController* ActivePC = UMyBlueprintFunctionLibrary::GetActivePlayerController(this))
		{
			if (ConnectionState == EInputDeviceConnectionState::Connected) // Our active controller connected
			{
				if (bDidContollerDisconnectPauseGame)
				{
					UGameplayStatics::SetGamePaused(this, false);
					bDidContollerDisconnectPauseGame = false;
				}
				ControllerDisconnectedWidget->RemoveFromParent();
			}
			else // Our active controller disconnected
			{
				if (ControllerDisconnectedWidgetClass)
				{
					if (!UGameplayStatics::IsGamePaused(this))
					{
						bDidContollerDisconnectPauseGame = true;
						UGameplayStatics::SetGamePaused(this, true);
					}
					ControllerDisconnectedWidget = CreateWidget<UControllerDisconnectedWidget>(ActivePC, ControllerDisconnectedWidgetClass);
					ControllerDisconnectedWidget->AddToViewport();
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ControllerDisconnectedWidgetClass NOT set!!!"));
				}

			}
		}
	}
}