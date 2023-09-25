// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnableObjects.h"
#include "Coin.generated.h"

/**
 * 
 */

class AEndlessRunnerCharacter;

UCLASS()
class ACoin : public ASpawnableObjects
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = Score)
		float ScorePerCoin = 10.f;

	UPROPERTY()
		AEndlessRunnerCharacter* Player;
	bool MagnetActive = false;
	FVector Destination = FVector(0, 0, 0);

	void BeginPlay() override;
	void ReactToTrigger(AActor* OtherActor) override;
	void Tick(float deltaTime) override;

	UFUNCTION()
	void ActivateMagnet(bool State);


};
