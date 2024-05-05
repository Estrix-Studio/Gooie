// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GlooObject.generated.h"

UCLASS()
class ESTRIX_API AGlooObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlooObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* BoxCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	bool isFullGrown = false; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Gloo")
	bool GetIsFullGrown() const;

};
