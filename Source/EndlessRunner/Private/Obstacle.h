// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnableObjects.h"
#include "Obstacle.generated.h"

/**
 * 
 */
UCLASS()
class AObstacle : public ASpawnableObjects
{
	GENERATED_BODY()

	void ReactToTrigger(AActor* OtherActor) override;

protected:
	UPROPERTY(EditAnywhere, Category = Score)
		float ImpulseAppliedToPlayer = 1500.0f;

	void TestCollisionDirection();
};
