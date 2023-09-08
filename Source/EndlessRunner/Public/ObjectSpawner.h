// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnableObjects.h"
#include "GameFramework/Actor.h"
#include "ObjectSpawner.generated.h"

UCLASS()
class ENDLESSRUNNER_API AObjectSpawner : public AActor
{
	GENERATED_BODY()
	

protected:
	TArray<ASpawnableObjects*> AllObjects;

	UPROPERTY(EditAnywhere) TSubclassOf<class ASpawnableObjects> SpawnableObject;

	UPROPERTY(EditAnywhere)  float SpawnCooldown = 2.0f;
	float timer = 10.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnObject();

public:	
	// Called every frame
	// Sets default values for this actor's properties
	AObjectSpawner();
	virtual void Tick(float DeltaTime) override;

};
