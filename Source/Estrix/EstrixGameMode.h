// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EstrixGameMode.generated.h"

UCLASS(minimalapi)
class AEstrixGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEstrixGameMode();

	virtual void BeginPlay() override;

};



