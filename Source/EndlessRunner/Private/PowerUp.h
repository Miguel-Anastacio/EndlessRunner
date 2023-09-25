// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnableObjects.h"
#include "PowerUp.generated.h"

class AEndlessRunnerCharacter;

/**
 * 
 */
UCLASS()
class APowerUp : public ASpawnableObjects
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	float Duration = 10.0f;
	
	virtual void ApplyEffect(AEndlessRunnerCharacter* player);
	void ReactToTrigger(AActor* otherActor) override;
	
};
