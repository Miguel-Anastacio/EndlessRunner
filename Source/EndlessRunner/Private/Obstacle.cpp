// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"

void AObstacle::ReactToTrigger(AActor* OtherActor)
{
	AEndlessRunnerCharacter* player = Cast<AEndlessRunnerCharacter>(OtherActor);
	if (player)
	{
		if (player->GetAlive())
		{

			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Hitting Obstacle"));
			float impulse = ImpulseAppliedToPlayer;
			if (player->GetCharacterMovement()->Velocity.Y < 0)
			{
				impulse = -ImpulseAppliedToPlayer;
			}
			else
			{
				impulse = ImpulseAppliedToPlayer;
			}
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Impulse base %f"), impulse));

			impulse += player->GetCharacterMovement()->Velocity.Y * 1.5;
			player->GetCharacterMovement()->AddImpulse(FVector(100, impulse, 0), true);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Impulse applied %f"), impulse));
			player->SetAlive(false);
			player->SetMovementState(AIRBORNE);

		}

	}
}
