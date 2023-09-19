// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"

void AObstacle::ReactToTrigger(AActor* OtherActor)
{
	AEndlessRunnerCharacter* player = Cast<AEndlessRunnerCharacter>(OtherActor);
	if (player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Hitting Obstacle"));
		float impulse = ImpulseAppliedToPlayer;
		if (player->GetActorLocation().Y < 0)
		{
			impulse = -ImpulseAppliedToPlayer;
		}
		else
		{
			impulse = ImpulseAppliedToPlayer;
		}

		player->GetCharacterMovement()->AddImpulse(FVector(0, impulse, 0), true);
		player->SetAlive(false);
		player->SetMovementState(AIRBORNE);
	}
}
