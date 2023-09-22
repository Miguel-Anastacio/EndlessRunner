// Fill out your copyright notice in the Description page of Project Settings.

#include "HorizontalPlatform.h"
#include "DrawDebugHelpers.h"
#include "ObjectSpawner.h"

AHorizontalPlatform::AHorizontalPlatform()
{
	CoinSpawn = CreateDefaultSubobject<USceneComponent>("Coin Spawn");
	CoinSpawn->SetupAttachment(ObjectMesh);
	ObstacleSpawnA = CreateDefaultSubobject<USceneComponent>("Obstacle Spawn A");
	ObstacleSpawnB = CreateDefaultSubobject<USceneComponent>("Obstacle Spawn B");
	ObstacleSpawnC = CreateDefaultSubobject<USceneComponent>("Obstacle Spawn C");
	ObstacleSpawnA->SetupAttachment(ObjectMesh);
	ObstacleSpawnB->SetupAttachment(ObjectMesh);
	ObstacleSpawnC->SetupAttachment(ObjectMesh);
}
void AHorizontalPlatform::SpawnCoinsAlongPlatform()
{	
	int numberOfCoinsToSpawn = FMath::RandRange(MinNumberOfCoinsPerPlatform, MaxNumberOfCoinsPerPlatform);
	if (numberOfCoinsToSpawn == 0)
	{
		return;
	}

	for (int i = 0; i < numberOfCoinsToSpawn; i++)
	{
		FVector location = CoinSpawn->GetComponentLocation();
		location += (i * SpawnOffset);
		ASpawnableObjects* coin = SpawnObjectOnLocation(location, Coin);
		AllObjects.Add(coin);
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
	SetAudioManager(AudioManager);

	ObstacleSpawn.Empty();
	ObstacleSpawn.Add(ObstacleSpawnA);
	ObstacleSpawn.Add(ObstacleSpawnB);
	ObstacleSpawn.Add(ObstacleSpawnC);


	for (int i = 0; i < NumberOfObstacles; i++)
	{
		if (ObstacleSpawn[i])
		{
			ASpawnableObjects* obj = SpawnObjectOnLocation(ObstacleSpawn[i]->GetComponentLocation(), Obstacle);
			AllObjects.Add(obj);
		}
	}

	if (UseAdaptableCoinSpawner)
	{
		SpawnCoins();

	}
	else
	{
		SpawnCoinsAlongPlatform();
	}
	SetVelocity(Velocity);
	SetAudioManager(AudioManager);

}

void AHorizontalPlatform::ReactToTrigger(AActor* OtherActor)
{

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
		if (it)
		{
			it->SetVelocity(speed);
		}
		else
		{
			AllObjects.Remove(it);
		}
	}
}

void AHorizontalPlatform::SetAudioManager(AAudioManager* audioMgr)
{
	AudioManager = audioMgr;
	for (auto it : AllObjects)
	{
		it->SetAudioManager(audioMgr);
	}
}

void AHorizontalPlatform::SpawnCoins()
{
	int numberOfCoinsToSpawn = FMath::RandRange(MinNumberOfCoinsPerPlatform, MaxNumberOfCoinsPerPlatform);
	if (numberOfCoinsToSpawn == 0)
	{
		return;
	}


	int coinCounter = 1;
	FVector location = DecideCoinClusterSpawnLocation();
	ASpawnableObjects* coin = SpawnObjectOnLocation(location, Coin);
	if (!IsCoinOverlappingWithOtherObjects(coin))
	{
		AllObjects.Add(coin);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Coin is on platform"));

		while (IsCoinOnPlatform(coin) && coinCounter < numberOfCoinsToSpawn)
		{
			location += SpawnOffset;
			coin = SpawnObjectOnLocation(location, Coin);
			while (IsCoinOverlappingWithOtherObjects(coin))
			{
				location += SpawnOffset * 2;
				coin->SetActorLocation(location);
			}
			AllObjects.Add(coin);
			coinCounter++;
		}

		if (!IsCoinOnPlatform(coin))
		{
			coin->Destroy();
		}

	}
	else
	{
		coin->Destroy();
		SpawnCoins();
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, FString::Printf(TEXT("Coins spawned %i"), coinCounter));
}

FVector AHorizontalPlatform::DecideCoinClusterSpawnLocation()
{
	FBox box = GetBoundingBoxOfActorInWorldSpace(this);
	FVector minWorldBound = box.Min;
	FVector maxWorldBound = box.Max;

	FVector RandomLocation;
	RandomLocation.X = FMath::RandRange(minWorldBound.X + TweakToSpawnLocationXAxis.X, maxWorldBound.X + TweakToSpawnLocationXAxis.Y);
	RandomLocation.Y = FMath::RandRange(minWorldBound.Y + TweakToSpawnLocationXAxis.Y, maxWorldBound.Y + TweakToSpawnLocationXAxis.Y);
	RandomLocation.Z = PositionCoinsZAxis;

	return RandomLocation;

}

bool AHorizontalPlatform::IsCoinOnPlatform(ASpawnableObjects* coin)
{
	FBox platformBox = GetBoundingBoxOfActorInWorldSpace(this);
	FBox coinBox = GetBoundingBoxOfActorInWorldSpace(coin);

	FTransform MyTransform;
	return platformBox.IsInsideXY(coinBox);
}

bool AHorizontalPlatform::IsCoinOverlappingWithOtherObjects(ASpawnableObjects* coin)
{
	FBox coinBox = GetBoundingBoxOfActorInWorldSpace(coin);

	FTransform MyTransform;
	for (auto it : AllObjects)
	{
		FBox box = GetBoundingBoxOfActorInWorldSpace(it);
		if (box.IntersectXY(coinBox))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, it->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Coin overlapping with other objects"));

			return true;
		}
	}

	return false;
}

FBox AHorizontalPlatform::GetBoundingBoxOfActorInWorldSpace(ASpawnableObjects* object)
{
	FVector minLocalBound = FVector(0, 0, 0);
	FVector maxLocalBound = FVector(0, 0, 0);

	object->GetObjectMesh()->GetLocalBounds(minLocalBound, maxLocalBound);
	FTransform transform = object->GetObjectMesh()->GetComponentTransform();

	FBox box = object->GetObjectMesh()->GetStaticMesh()->GetBounds().GetBox();
	box.Min = minLocalBound * transform.GetScale3D() + object->GetActorLocation();
	box.Max = maxLocalBound * transform.GetScale3D() + object->GetActorLocation();

	return box;
}
