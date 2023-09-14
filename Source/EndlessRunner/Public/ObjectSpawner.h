// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HorizontalPlatform.h"
#include "GameFramework/Actor.h"
#include "ObjectSpawner.generated.h"

UENUM(BlueprintType)
enum WallSpawn
{
	HORIZONTAL UMETA(DisplayName = "HORIZONTAL"),
	WALL_RIGHT UMETA(DisplayName = "WALL RIGTH"),
	WALL_LEFT UMETA(DisplayName = "WALL LEFT")
};


UCLASS()
class ENDLESSRUNNER_API AObjectSpawner : public AActor
{
	GENERATED_BODY()
	

protected:

	// positions for spawning walls
	UPROPERTY(EditAnywhere) USceneComponent* HorizontalTransform;
	UPROPERTY(EditAnywhere) USceneComponent* WallRightTransform;
	UPROPERTY(EditAnywhere) USceneComponent* WallLeftTransform;

	// keeps track of wall spawns
	WallSpawn PreviousWallSpawn = HORIZONTAL;
	WallSpawn CurrentWallSpawn = HORIZONTAL;

	// used to spawn the second wall
	// then we use the triggers on each wall
	bool IsFirstWall = true;

	TArray<ASpawnableObjects*> AllObjects;

	UPROPERTY(EditAnywhere) TArray<TSubclassOf<class AHorizontalPlatform>> SpawnableObject;

	UPROPERTY(EditAnywhere)  float SpawnCooldown = 2.0f;
	float timer = 0.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Spawns a wall depending on what type was decided

	ASpawnableObjects* SpawnObject();

	// decides whic type of wall to spawn
	void DecideWallToSpawn();


public:	
	// Sets default values for this actor's properties
	AObjectSpawner();

	virtual void Tick(float DeltaTime) override;

	void SpawnWall();

	bool GetIsFirstWall() {
		return IsFirstWall;
	};

};
