// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "GlooObject.h"
#include "SprayObject.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EstrixProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class AEstrixProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<AGlooObject> GlooClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<ASprayObject> SprayClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float CollisionOffset = 28;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	UMaterial* Decal;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	float DecalSize;

	UPROPERTY(EditDefaultsOnly, Category = "Decal")
	float DecalDepth;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool isSpraying = false;
public:
	AEstrixProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void CreateGlooObject(AActor* OtherActor,
		UPrimitiveComponent* OtherComp, const FHitResult& Hit) const;

	void CreateSprayObject(AActor* OtherActor,
		UPrimitiveComponent* OtherComp, const FHitResult& Hit) const;

	void SpawnSprayDecal(const FHitResult& Hit, AActor* SprayObject) const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SetSpraying();

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }

	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

