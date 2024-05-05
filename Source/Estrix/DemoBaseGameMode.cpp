// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoBaseGameMode.h"
#include "DemoBasePlayerController.h"

ADemoBaseGameMode::ADemoBaseGameMode()
	: Super()
{
	PlayerControllerClass = ADemoBasePlayerController::StaticClass();
}
