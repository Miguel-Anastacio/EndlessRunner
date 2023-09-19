// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnableObjects.h"
#include "SpawningInterface.h"
#include "HorizontalPlatform.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AHorizontalPlatform : public ASpawnableObjects, public ISpawningInterface
{
	GENERATED_BODY()

public:
	AHorizontalPlatform();

protected:
	UPROPERTY(EditAnywhere)
		float Length = 20.f;
	UPROPERTY(EditAnywhere)
		bool HasTriggeredSpawn = false;

	UPROPERTY(EditAnywhere, Category = Coin)
		TSubclassOf<class ASpawnableObjects> Coin;
	UPROPERTY(EditAnywhere, Category = Coin)
		USceneComponent* CoinSpawn;
	UPROPERTY(EditAnywhere, Category = Coin)
		FVector SpawnOffset = FVector(100, 0, 0);

	UPROPERTY(EditAnywhere, Category = Obstacle)
	TSubclassOf<class ASpawnableObjects> Obstacle;
	UPROPERTY(EditAnywhere, Category = Obstacle)
		USceneComponent* ObstacleSpawn;

	UPROPERTY()
	TArray<ASpawnableObjects*> AllObjects;

	void SpawnCoinsAlongPlatform(const int NumberOfCoins);
	ASpawnableObjects* SpawnObjectOnLocation(const FVector location, TSubclassOf<class ASpawnableObjects> objectToSpawn) override;

public:
	void BeginPlay() override;
	void ReactToTrigger(AActor* OtherActor) override;
	void SetVelocity(float speed) override;
};
