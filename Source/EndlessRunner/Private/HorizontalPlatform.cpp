// Fill out your copyright notice in the Description page of Project Settings.

#include "HorizontalPlatform.h"
#include "ObjectSpawner.h"
AHorizontalPlatform::AHorizontalPlatform()
{
	CoinSpawn = CreateDefaultSubobject<USceneComponent>("Coin Spawn");
	CoinSpawn->SetupAttachment(ObjectMesh);

	ObstacleSpawn = CreateDefaultSubobject<USceneComponent>("Obstacle Spawn");
	ObstacleSpawn->SetupAttachment(ObjectMesh);
}
void AHorizontalPlatform::SpawnCoinsAlongPlatform(const int NumberOfCoins)
{
	FBoxSphereBounds bounds = ObjectMesh->GetStaticMesh()->GetBounds();
	FVector extent = bounds.GetBox().GetExtent();
	
	for (int i = 0; i < NumberOfCoins; i++)
	{
		FVector location = CoinSpawn->GetComponentLocation();
		location += (i * SpawnOffset);
		ASpawnableObjects* coin = SpawnObjectOnLocation(location, Coin);
	}

}
ASpawnableObjects* AHorizontalPlatform::SpawnObjectOnLocation(const FVector location, TSubclassOf<class ASpawnableObjects> objectToSpawn)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(location);
	if (objectToSpawn)
	{
		ASpawnableObjects* object = GetWorld()->SpawnActor<ASpawnableObjects>(objectToSpawn, SpawnTransform, SpawnParams);
		object->SetVelocity(Velocity);
		AllObjects.Add(object);
		return object;
	}
	else
	{
		return NULL;
	}
}

void AHorizontalPlatform::BeginPlay()
{
	Super::BeginPlay();
	SpawnCoinsAlongPlatform(3);
	SpawnObjectOnLocation(ObstacleSpawn->GetComponentLocation(), Obstacle);
}

void AHorizontalPlatform::ReactToTrigger(AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, TEXT("Overlap"));

	/*
	if(GetOwner() == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Owner is NULLLLLL"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, GetOwner()->GetName());
	}
	*/
	if (GetOwner() == NULL)
		return;

	AObjectSpawner* ObjectSpawner = Cast<AObjectSpawner>(GetOwner());
	if (!HasTriggeredSpawn)
	{
		if (ObjectSpawner)
		{
			if (!ObjectSpawner->GetIsFirstWall())
			{
				ObjectSpawner->SpawnWall();
				HasTriggeredSpawn = true;
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Object Spawner is NULLLLLL"));
			HasTriggeredSpawn = true;
		}
	}
}

void AHorizontalPlatform::SetVelocity(float speed)
{
	Velocity = speed;
	for (auto it : AllObjects)
	{
		it->SetVelocity(speed);
	}
}
