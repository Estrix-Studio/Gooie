// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DemoBasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ESTRIX_API ADemoBasePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;

public:

	virtual bool InputKey(const FInputKeyParams& Params) override;
};
