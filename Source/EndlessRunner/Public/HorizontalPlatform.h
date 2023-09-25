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
	UPROPERTY(EditAnywhere, Category = Coin)
		int32 MaxNumberOfCoinsPerPlatform = 5;
	UPROPERTY(EditAnywhere, Category = Coin)
		int32 MinNumberOfCoinsPerPlatform = 5;
	UPROPERTY(EditAnywhere, Category = Coin)
		int32 ClustersOfCoins = 2;

	UPROPERTY(EditAnywhere, Category = Coin)
		bool UseAdaptableCoinSpawner = false;

	/* values to adjust the spawn range of coins on the X axis
	   X - minimum position adjustment
	   Y - maximum position adjustment*/
	UPROPERTY(EditAnywhere, Category = Coin)
		FVector2D TweakToSpawnLocationXAxis = FVector2D(-100, 100);
	/* values to adjust the spawn range of coins on the Y axis
	   X - minimum position adjustment
	   Y - maximum position adjustment*/
	UPROPERTY(EditAnywhere, Category = Coin)
		FVector2D TweakToSpawnLocationYAxis = FVector2D(-100, 100);
	/* valueto fix the height of coins */
	UPROPERTY(EditAnywhere, Category = Coin)
		float PositionCoinsZAxis = 140.0f;

	UPROPERTY(EditAnywhere, Category = PowerUp)
		TArray <TSubclassOf<class ASpawnableObjects>> PowerUps;
	UPROPERTY(EditAnywhere, Category = PowerUp)
		float PowerUpChance = 100.0f;


	UPROPERTY(EditAnywhere, Category = Obstacle)
	TSubclassOf<class ASpawnableObjects> Obstacle;
	UPROPERTY(EditAnywhere, Category = Obstacle)
		TArray<USceneComponent*> ObstacleSpawn;

	UPROPERTY(EditAnywhere, Category = Obstacle)
		USceneComponent* ObstacleSpawnA;
	UPROPERTY(EditAnywhere, Category = Obstacle)
		USceneComponent* ObstacleSpawnB;
	UPROPERTY(EditAnywhere, Category = Obstacle)
		USceneComponent* ObstacleSpawnC;

	UPROPERTY(EditAnywhere, Category = Obstacle)
		int NumberOfObstacles = 0;


	UPROPERTY()
	TArray<ASpawnableObjects*> AllObjects;

	void SpawnCoinsAlongPlatform();
	ASpawnableObjects* SpawnObjectOnLocation(const FVector location, TSubclassOf<class ASpawnableObjects> objectToSpawn) ;

	void SpawnCoins();
	FVector RandomSpawnLocationInsidePlatform();
	bool IsCoinOnPlatform(ASpawnableObjects* coin);
	bool IsCoinOverlappingWithOtherObjects(ASpawnableObjects* coin);
	FBox GetBoundingBoxOfActorInWorldSpace(ASpawnableObjects* object);


	void SpawnPowerUp();

public:
	void BeginPlay() override;
	void ReactToTrigger(AActor* OtherActor) override;
	void SetVelocity(float speed) override;
	void SetAudioManager(AAudioManager* audioMgr) override;

};
