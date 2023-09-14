// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnableObjects.h"

// Sets default values
ASpawnableObjects::ASpawnableObjects()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root Component"));

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMesh");
	ObjectMesh->SetupAttachment(RootComponent);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>("Trigger");
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->InitBoxExtent(FVector(20.0f, 20.0f, 20.0f));
}


// Called when the game starts or when spawned
void ASpawnableObjects::BeginPlay()
{
	Super::BeginPlay();
	if(TriggerBox)
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASpawnableObjects::OnOverlapBegin);
}

// Called every frame
void ASpawnableObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector currentLocation = GetActorLocation();
	currentLocation.X = currentLocation.X + Velocity * DeltaTime;

	SetActorLocation(currentLocation);

	DrawDebugBox(GetWorld(), TriggerBox->GetComponentLocation(), TriggerBox->GetScaledBoxExtent(), FColor::Purple, false, .1, 0, 5);

	if (currentLocation.X > 3000.0f)
		Destroy();
}

void ASpawnableObjects::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		ReactToTrigger();
		UE_LOG(LogTemp, Warning, TEXT("Overlap"));
	}
}


void ASpawnableObjects::ReactToTrigger()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Overlap Begun "));
}

