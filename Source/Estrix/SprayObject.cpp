#include "SprayObject.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASprayObject::ASprayObject()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Set up components
    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    // ... Setup other components

    // Set default values
    isFullGrown = false;
    bIsOnFire = false;
    FireSpreadRadius = 200.0f;  // You can adjust this value based on your needs
    BurningTime = 10.0f;
    SetOnFireDelayTime = .5f;
}

// Called when the game starts or when spawned
void ASprayObject::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ASprayObject::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Your tick logic here

    if (bIsOnFire)
    {
        // Update the burning time
        BurningTime -= DeltaTime;

        if (BurningTime <= 0.0f)
        {
            // Time is up, destroy the object
            Destroy();
        }
    }
}

bool ASprayObject::GetIsFullGrown() const
{
    return isFullGrown;
}

void ASprayObject::SetOnFire()
{
    if (!bIsOnFire)
    {
        bIsOnFire = true;
       
    }
    if (FireMaterial)
    {
        // StaticMesh->SetMaterial(0, FireMaterial);
    }
    SpreadFireToNearbyObjects();
}

void ASprayObject::SpreadFireToNearbyObjects()
{
    TArray<AActor*> NearbyObjects;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASprayObject::StaticClass(), NearbyObjects);

    for (AActor* NearbyObject : NearbyObjects)
    {
        ASprayObject* OtherSprayObject = Cast<ASprayObject>(NearbyObject);
        if (OtherSprayObject && !OtherSprayObject->bIsOnFire)
        {
            float Distance = FVector::Dist(GetActorLocation(), OtherSprayObject->GetActorLocation());
            if (Distance <= FireSpreadRadius)
            {
                OtherSprayObject->SetOnFireWithDelay(SetOnFireDelayTime);
            }
        }
    }
}

void ASprayObject::SetOnFireWithDelay(float Delay)
{
    bIsOnFire;
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ASprayObject::SetOnFire, Delay, false);
}

bool ASprayObject::GetIsOnFire()
{
    return bIsOnFire;
}


