// Fill out your copyright notice in the Description page of Project Settings.

#include "Coin.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"

void ACoin::ReactToTrigger(AActor* OtherActor)
{
	AEndlessRunnerCharacter* player = Cast<AEndlessRunnerCharacter>(OtherActor);
	if (player)
	{
		player->IncreaseScore(ScorePerCoin);
		Destroy();
	}
}
