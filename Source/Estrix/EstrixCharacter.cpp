// Copyright Epic Games, Inc. All Rights Reserved.

#include "EstrixCharacter.h"
#include "EstrixProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Controller.h"
#include "TP_WeaponComponent.h"





//////////////////////////////////////////////////////////////////////////
// AEstrixCharacter

AEstrixCharacter::AEstrixCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
}




void AEstrixCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();


}


//////////////////////////////////////////////////////////////////////////// Input

void AEstrixCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{

			if (UGameplayStatics::GetPlatformName() == "SWITCH")
			{
				Subsystem->AddMappingContext(Default_SW_MappingContext, 0);
			}
			else
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);

			}
		}
	}




	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AEstrixCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AEstrixCharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEstrixCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEstrixCharacter::Look);

		//Pausing
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Completed, this, &AEstrixCharacter::Pause);

		EnhancedInputComponent->BindAction(BackAction, ETriggerEvent::Completed, this, &AEstrixCharacter::Back);
		
		//back
		EnhancedInputComponent->BindAction(BackActionRight, ETriggerEvent::Completed, this, & AEstrixCharacter::BackRight);
	
		EnhancedInputComponent->BindAction(BackActionBottom, ETriggerEvent::Completed, this, &AEstrixCharacter::BackButtom);

	}
}


void AEstrixCharacter::Jump(const FInputActionValue& Value)
{
	ACharacter::Jump();
}
void AEstrixCharacter::StopJumping(const FInputActionValue& Value)
{
	ACharacter::StopJumping();
}




void AEstrixCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AEstrixCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AEstrixCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AEstrixCharacter::GetHasRifle()
{
	return bHasRifle;
}

void AEstrixCharacter::Pause()
{
	if (!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		if (PauseMenuWidgetClass)
		{
			PauseMenuWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass);

			if (PauseMenuWidget)
			{

				UGameplayStatics::SetGamePaused(GetWorld(), true);
				PauseMenuWidget->AddToViewport();

				FInputModeGameAndUI InputMode;
				APlayerController* PC = Cast<APlayerController>(GetController());
				PC->SetInputMode(InputMode);

			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT(" Charachter Pause: PauseMenuWidget not valid"));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Charachter does not have PauseMenuWidgetClass set!!!"));
		}

	}
	else
	{
		Back();

	}
}

void AEstrixCharacter::BackRight()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BackRight"));

#if PLATFORM_WINDOWS
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You're on WINDOWS"));
	return;
#elif PLATFORM_PS5
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You're on PS5"));
#elif PLATFORM_SWITCH
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You're on SWITCH"));
	return;
#else
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You're on SWITCH"));

#endif
	Back();
}

void AEstrixCharacter::BackButtom()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BackButton"));

#if PLATFORM_WINDOWS
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You're on WINDOWS"));

#elif PLATFORM_PS5
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You're on PS5"));
	return;

#elif PLATFORM_SWITCH
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You're on SWITCH"));
	return;

#else
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("You're on SWITCH"));

#endif
	Back();
}

void AEstrixCharacter::Back()
{

	if (UGameplayStatics::IsGamePaused(GetWorld()) )
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		if (PauseMenuWidget)
		{
			PauseMenuWidget->RemoveFromParent();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Charachter Pause: PauseMenuWidget not valid"));
			return;
		}

		FInputModeGameOnly InputMode;
		APlayerController* PC = Cast<APlayerController>(GetController());
		PC->SetInputMode(InputMode);


	}
}

