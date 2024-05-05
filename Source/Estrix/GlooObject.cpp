// Fill out your copyright notice in the Description page of Project Settings.


#include "GlooObject.h"

// Sets default values
AGlooObject::AGlooObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Block);
	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetBoxExtent(FVector(28));
	RootComponent = BoxCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetupAttachment(BoxCollision);

}

// Called when the game starts or when spawned
void AGlooObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGlooObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGlooObject::GetIsFullGrown() const
{
	return isFullGrown;
}

