// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectSpawner.h"

// Sets default values
AObjectSpawner::AObjectSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AObjectSpawner::SpawnObject()
{
	FTransform SpawnTransform;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnTransform.SetRotation(FQuat4d(0,0,0,1.0f));
	SpawnTransform.SetLocation(GetActorLocation());
	
	ASpawnableObjects* temp = GetWorld()->SpawnActor<ASpawnableObjects>(SpawnableObject, SpawnTransform, SpawnParams);
	AllObjects.Add(temp);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned"));
}

// Called every frame
void AObjectSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (timer > SpawnCooldown)
	{
		SpawnObject();
		timer = 0.0f;
	}
	timer += DeltaTime;
}

