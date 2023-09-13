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
	
	int index = FMath::RandRange(0, SpawnableObject.Num() - 1);

	ASpawnableObjects* temp = GetWorld()->SpawnActor<ASpawnableObjects>(SpawnableObject[index], SpawnTransform, SpawnParams);

	if (temp->ActorHasTag("Vertical Wall"))
	{
		if (FMath::RandBool())
		{
			temp->SetActorLocation(WallRightTransform->GetComponentLocation());
			CurrentWallSpawn = WALL_RIGHT;

		}
		else
		{
			CurrentWallSpawn = WALL_LEFT;
			temp->SetActorLocation(WallLeftTransform->GetComponentLocation());
		}
	}
	else
	{
		CurrentWallSpawn = HORIZONTAL;
	}

	return temp;
}

void AObjectSpawner::SpawnWall()
{
	ASpawnableObjects* object = SpawnObject();
	if (CurrentWallSpawn != PreviousWallSpawn)
	{
		AllObjects.Add(object);
		PreviousWallSpawn = CurrentWallSpawn;
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawned"));
	}
	else
	{
		object->Destroy();
		SpawnWall();
	}
}

// Called every frame
void AObjectSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (timer > SpawnCooldown)
	{
		SpawnWall();
		timer = 0.0f;
	}
	timer += DeltaTime;
}

