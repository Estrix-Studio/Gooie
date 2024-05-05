// Copyright Epic Games, Inc. All Rights Reserved.

#include "EstrixProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GlooObject.h"
#include "Components/DecalComponent.h"
#include "EstrixCharacter.h"


AEstrixProjectile::AEstrixProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(2.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AEstrixProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
    
	//Set up GlooClass

	// Die after 3 seconds by default



	InitialLifeSpan = 0.5f;
}

void AEstrixProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{


	// Only add impulse and destroy projectile if we hit a physics
	if (isSpraying)
	{
		if (SprayClass)
		{

			const AEstrixCharacter* OtherCharacter = Cast<AEstrixCharacter>(OtherActor);
			if (OtherCharacter)
			{
				// Apply Gloo Effect to Character
			}
			else
			{
				CreateSprayObject(OtherActor, OtherComp, Hit);
			}
		}
	}
	else
	{
		if (GlooClass && !OtherActor->IsA(GlooClass))
		{

			const AEstrixCharacter* OtherCharacter = Cast<AEstrixCharacter>(OtherActor);
			if (OtherCharacter)
			{
				// Apply Gloo Effect to Character
			}
			else
			{
				CreateGlooObject(OtherActor, OtherComp, Hit);
			}
		}
		else if (GlooClass && OtherActor->IsA(GlooClass))
		{
			const AGlooObject* OtherGlooObject = Cast<AGlooObject>(OtherActor);
			if (OtherGlooObject && OtherGlooObject->GetIsFullGrown())
			{
				// Create GlooObject
				CreateGlooObject(OtherActor, OtherComp, Hit);
			}
		}
	}


	//Destroy();
}


void AEstrixProjectile::CreateGlooObject(AActor* OtherActor,
	UPrimitiveComponent* OtherComp, const FHitResult& Hit) const
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FRotator SpawnRotation = FRotationMatrix::MakeFromZ(Hit.Normal).Rotator();
	const FVector SpawnLocation = Hit.ImpactPoint + (Hit.ImpactNormal * CollisionOffset);

	AActor* Gloo = GetWorld()->SpawnActor<AActor>(
		GlooClass, SpawnLocation, SpawnRotation, SpawnParameters
	);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Stick"));

	// Set up attachment rules
	const FAttachmentTransformRules AttachmentTransformRules =
		FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);

	// Attach Gloo to the actor
	Gloo->AttachToComponent(OtherComp, AttachmentTransformRules);
	if (Gloo && OtherActor && OtherComp->IsSimulatingPhysics())
	{
		

		// Apply an impulse to the attached actor
		OtherComp->AddImpulseAtLocation(-GetVelocity(), Hit.Location);
	}
}

void AEstrixProjectile::CreateSprayObject(AActor* OtherActor,
	UPrimitiveComponent* OtherComp, const FHitResult& Hit) const
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FRotator SpawnRotation = FRotationMatrix::MakeFromZ(Hit.Normal).Rotator();
	const FVector SpawnLocation = Hit.ImpactPoint + (Hit.ImpactNormal * CollisionOffset);

	AActor* Spray = GetWorld()->SpawnActor<AActor>(
		SprayClass, SpawnLocation, SpawnRotation, SpawnParameters
	);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Sprayed"));

	// Set up attachment rules
	const FAttachmentTransformRules AttachmentTransformRules =
		FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);

	// Attach Gloo to the actor
	Spray->AttachToComponent(OtherComp, AttachmentTransformRules);
	SpawnSprayDecal(Hit, Spray);
	if (Spray && OtherActor && OtherComp->IsSimulatingPhysics())
	{


		// Apply an impulse to the attached actor
		OtherComp->AddImpulseAtLocation(-GetVelocity(), Hit.Location);
	}
}

void AEstrixProjectile::SpawnSprayDecal(const FHitResult& Hit, AActor* SprayObject) const
{
	const FVector Location = Hit.ImpactPoint;
	FRotator RandomRollRotation = Hit.ImpactNormal.Rotation();
	RandomRollRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);

	const float RandomDecalSize = DecalSize * FMath::FRandRange(0.75f, 1.15f);
	const FVector DecalBounds = FVector(DecalDepth, RandomDecalSize, RandomDecalSize);

	UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAttached
	(
		Decal, DecalBounds, SprayObject->GetRootComponent(), NAME_None, Location,
		RandomRollRotation, EAttachLocation::KeepWorldPosition
	);

	DecalComponent->SetFadeIn(0.25f, 0.4f);
}


void AEstrixProjectile::SetSpraying()
{
	isSpraying = true;
}
