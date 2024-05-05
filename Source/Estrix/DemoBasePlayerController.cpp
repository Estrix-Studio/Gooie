// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoBasePlayerController.h"


void ADemoBasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		this->ConsoleCommand(FString::Printf(TEXT("r.DebugSafeZone.TitleRatio %f"), 0.9));
	}
}

bool ADemoBasePlayerController::InputKey(const FInputKeyParams& Params)
{
	if (Params.Event == EInputEvent::IE_Pressed && Params.IsGamepad())
	{
		UE_LOG(LogTemp, Warning, TEXT("Gamepad Button %s was pressed!"), *Params.Key.ToString());
	}

	return Super::InputKey(Params);
}
