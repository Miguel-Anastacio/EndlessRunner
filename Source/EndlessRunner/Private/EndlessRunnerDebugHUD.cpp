// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRunnerDebugHUD.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

/**
Draw the HUD.
*********************************************************************************/

void AEndlessRunnerDebugHUD::DrawHUD()
{
	Super::DrawHUD();

	AEndlessRunnerCharacter* player = Cast<AEndlessRunnerCharacter>(GetOwningPawn());
	if (player)
	{
		// Values of tilting phone
		AddFloat(TEXT("Gravity X:"), player->Gravity.X);
		AddFloat(TEXT("Gravity Y:"), player->Gravity.Y);
		AddFloat(TEXT("Gravity Z:"), player->Gravity.Z);

		FVector velocity = player->GetCharacterMovement()->Velocity;
		AddFloat(TEXT("velocity X:"), velocity.X);
		AddFloat(TEXT("velocity Y:"), velocity.Y);
		AddFloat(TEXT("velocity Z:"), velocity.Z);

		AddFloat(TEXT("Input X axis:"), player->InputXAxis);
		AddFloat(TEXT("Input Y axis:"), player->InputYAxis);
	}
	
}