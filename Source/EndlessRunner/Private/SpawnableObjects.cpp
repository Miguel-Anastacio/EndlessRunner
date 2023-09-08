// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnableObjects.h"

// Sets default values
ASpawnableObjects::ASpawnableObjects()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>("ObjectMesh");
	ObjectMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASpawnableObjects::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ASpawnableObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector currentLocation = GetActorLocation();
	currentLocation.X = currentLocation.X + Velocity * DeltaTime;
	
	SetActorLocation(currentLocation);


	
}

