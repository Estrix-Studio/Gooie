// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_WeaponComponent.generated.h"

class AEstrixCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESTRIX_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AEstrixProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FRotator SightRotationOffset;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprayAction;

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AEstrixCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire(bool isPraying);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartAutoFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopAutoFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AutoFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartSpraying();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopSpraying();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Spraying();

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	AEstrixCharacter* Character;

	bool bIsAutoFiring = false;

	// Timer handle for auto-firing
	FTimerHandle AutoFireTimerHandle;

	// Variable to control the firing rate (shots per second)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float RateOfFire = 2.0f; // Adjust the value based on your desired firing rate

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float RateOfSpray = 2.0f; // Adjust the value based on your desired firing rate
};
