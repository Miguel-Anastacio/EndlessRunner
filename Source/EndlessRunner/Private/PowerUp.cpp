// Fill out your copyright notice in the Description page of Project Settings.

#include "PowerUp.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"

void APowerUp::ApplyEffect(AEndlessRunnerCharacter* player)
{
	player->InitMagnet(Duration, true);
	player->MagnetActivatedDelegate.Broadcast(true);
}

void APowerUp::ReactToTrigger(AActor* otherActor)
{
	AEndlessRunnerCharacter* player = Cast<AEndlessRunnerCharacter>(otherActor);
	if (player)
	{
		ApplyEffect(player);
		player->GetMagnetMesh()->bHiddenInGame = false;
	}
}


