// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "EstrixCharacter.h"
#include "EstrixProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(150.0f, 100.0f, 30.0f);
	SightRotationOffset = FRotator(0.0f, 0.0f, -5.0f);

}


void UTP_WeaponComponent::Fire(bool isPraying)
{
	if (Character == nullptr || Character->GetController() == nullptr )
	{
		return;
	}


	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation()+ SightRotationOffset;
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// Spawn the projectile at the muzzle
			AEstrixProjectile* SpawnedProjectile = World->SpawnActor<AEstrixProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (SpawnedProjectile != nullptr)
			{
				// Call StartPraying if the isPraying flag is true
				if (isPraying)
				{
					SpawnedProjectile->SetSpraying();
				}
			}
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::AttachWeapon(AEstrixCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent) )
		{
			if (Character->GetWorld()->GetName().Contains("Screen"))
			{
				return;
			}
			// Fire
			//EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::StartAutoFire);

			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::StopAutoFire);

			EnhancedInputComponent->BindAction(SprayAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::StartSpraying);

			EnhancedInputComponent->BindAction(SprayAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::StopSpraying);

		}
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}
void UTP_WeaponComponent::StartAutoFire()
{
	// Start auto-firing when the left mouse button is pressed
	bIsAutoFiring = true;
	AutoFire();
}

void UTP_WeaponComponent::StopAutoFire()
{
	// Stop auto-firing when the left mouse button is released
	bIsAutoFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(AutoFireTimerHandle);
}

void UTP_WeaponComponent::AutoFire()
{
	// Check if auto-firing is enabled

	if (bIsAutoFiring)
	{
		// Implement the logic for firing here
		Fire(false);

		// Schedule the next shot based on your desired firing rate
		const float NextShotDelay = 1.0f / RateOfFire;
		GetWorld()->GetTimerManager().SetTimer(AutoFireTimerHandle, this, &UTP_WeaponComponent::AutoFire, NextShotDelay, false);
	}
}

void UTP_WeaponComponent::StartSpraying()
{
	// Start auto-firing when the left mouse button is pressed
	bIsAutoFiring = true;
	Spraying();
}

void UTP_WeaponComponent::StopSpraying()
{
	// Stop auto-firing when the left mouse button is released
	bIsAutoFiring = false;
	GetWorld()->GetTimerManager().ClearTimer(AutoFireTimerHandle);
}

void UTP_WeaponComponent::Spraying()
{
	// Check if auto-firing is enabled

	if (bIsAutoFiring)
	{
		// Implement the logic for firing here
		Fire(true);

		// Schedule the next shot based on your desired firing rate
		const float NextShotDelay = 1.0f / RateOfSpray;
		GetWorld()->GetTimerManager().SetTimer(AutoFireTimerHandle, this, &UTP_WeaponComponent::Spraying, NextShotDelay, false);
	}
}
