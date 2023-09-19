// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnableObjects.h"
#include "Coin.generated.h"

/**
 * 
 */
UCLASS()
class ACoin : public ASpawnableObjects
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = Score)
		float ScorePerCoin = 10.f;
	void ReactToTrigger(AActor* OtherActor) override;
};
