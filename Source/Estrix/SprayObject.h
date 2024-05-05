// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SprayObject.generated.h"


UCLASS()
class ESTRIX_API ASprayObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprayObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	bool isFullGrown = false;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	UMaterial* FireMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float FireSpreadRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fire")
	bool bIsOnFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float BurningTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
	float SetOnFireDelayTime;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UFUNCTION(BlueprintCallable, Category = "Spray")
	bool GetIsFullGrown() const;

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void SetOnFire();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void SpreadFireToNearbyObjects();

	UFUNCTION(BlueprintCallable, Category = "Fire")
	void SetOnFireWithDelay(float Delay);

	UFUNCTION(BlueprintCallable, Category = "Fire")
	bool GetIsOnFire();

};
