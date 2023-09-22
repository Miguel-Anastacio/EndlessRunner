// Fill out your copyright notice in the Description page of Project Settings.

#include "Coin.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"
#include "Components/AudioComponent.h"

void ACoin::ReactToTrigger(AActor* OtherActor)
{
	AEndlessRunnerCharacter* player = Cast<AEndlessRunnerCharacter>(OtherActor);
	if (player)
	{
		player->IncreaseScore(ScorePerCoin);
		if(AudioManager)
			AudioManager->CoinCollectedAudioComponent->Play();
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("AudioManager is null"));
		}
		Destroy();

	}
}
