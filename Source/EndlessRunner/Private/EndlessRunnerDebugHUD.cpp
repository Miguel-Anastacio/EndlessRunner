// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRunnerDebugHUD.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"

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
	}
	
}