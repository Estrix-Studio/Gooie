// Copyright Epic Games, Inc. All Rights Reserved.

#include "EstrixGameMode.h"

#include "DemoPlayerController.h"
#include "EstrixCharacter.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "MyBlueprintFunctionLibrary.h"
#include "Engine/World.h"


AEstrixGameMode::AEstrixGameMode()
	: Super()
{
	DefaultPawnClass = AEstrixCharacter::StaticClass();
	PlayerControllerClass = ADemoPlayerController::StaticClass();
}

void AEstrixGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld()->GetMapName().Contains("Screen"))
	{
		UMyGameInstance* GameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(this));
		if (GameInstanceRef)
		{
			if (GameInstanceRef->GetActiveControllerID() > -1)
			{

				APlayerController* PC = UGameplayStatics::CreatePlayer(this, GameInstanceRef->GetActiveControllerID());

				FInputModeGameOnly InputMode;
				PC->SetInputMode(InputMode);
				GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, TEXT("ControllerID is set!"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, TEXT("ControllerID not set!"));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, TEXT("GameInstance not set!"));
		}
	}

}

