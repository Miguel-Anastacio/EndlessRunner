// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectSpawner.h"

// Sets default values
AObjectSpawner::AObjectSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HorizontalTransform = CreateDefaultSubobject<USceneComponent>("HorizontalSpawn");
	RootComponent = HorizontalTransform;
	WallRightTransform = CreateDefaultSubobject<USceneComponent>("RightWallSpawn");
	WallRightTransform->SetupAttachment(HorizontalTransform);
	WallLeftTransform = CreateDefaultSubobject<USceneComponent>("LeftWallSpawn");
	WallLeftTransform->SetupAttachment(HorizontalTransform);
}

// Called when the game starts or when spawned
void AObjectSpawner::BeginPlay()
{

	Super::BeginPlay();
}

ASpawnableObjects* AObjectSpawner::SpawnObject()
{
	FTransform SpawnTransform;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnTransform.SetRotation(FQuat4d(0,0,0,1.0f));
	SpawnTransform.SetLocation(GetActorLocation());

	AHorizontalPlatform* temp = nullptr;

	switch (CurrentWallSpawn)
	{
	case HORIZONTAL:
		SpawnTransform.SetLocation(HorizontalTransform->GetComponentLocation());
		temp = GetWorld()->SpawnActor<AHorizontalPlatform>(SpawnableObject[0], SpawnTransform, SpawnParams);
		break;
	case WALL_LEFT:
		SpawnTransform.SetLocation(WallLeftTransform->GetComponentLocation());
		temp = GetWorld()->SpawnActor<AHorizontalPlatform>(SpawnableObject[1], SpawnTransform, SpawnParams);
		break;
	case WALL_RIGHT:
		SpawnTransform.SetLocation(WallRightTransform->GetComponentLocation());
		temp = GetWorld()->SpawnActor<AHorizontalPlatform>(SpawnableObject[1], SpawnTransform, SpawnParams);
		break;
	default:
		break;
	}
	return temp;
}

void AObjectSpawner::DecideWallToSpawn()
{
	switch (PreviousWallSpawn)
	{
	case HORIZONTAL:
		CurrentWallSpawn = StaticCast<WallSpawn>(FMath::RandRange(1, 2));
		break;
	case WALL_LEFT:
		CurrentWallSpawn = StaticCast<WallSpawn>(FMath::RandRange(0, 1));
		break;
	case WALL_RIGHT:
		CurrentWallSpawn = StaticCast<WallSpawn>(FMath::RandRange(0, 2));
		while (CurrentWallSpawn == PreviousWallSpawn)
		{
			CurrentWallSpawn = StaticCast<WallSpawn>(FMath::RandRange(0, 2));
		}
		break;
	default:
		break;
	}
}

void AObjectSpawner::SpawnWall()
{
	DecideWallToSpawn();
	AllObjects.Add(SpawnObject());
	PreviousWallSpawn = CurrentWallSpawn;
}

// Called every frame
void AObjectSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsFirstWall && timer > SpawnCooldown)
	{
		SpawnWall();
		timer = 0.0f;
		IsFirstWall = false;
	}
	timer += DeltaTime;
	
}

