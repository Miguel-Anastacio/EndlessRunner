// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessRunnerGameMode.generated.h"

UCLASS(minimalapi)
class AEndlessRunnerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEndlessRunnerGameMode();

	UFUNCTION(BlueprintCallable)
	void SetSpawnType();

	UPROPERTY(EditAnywhere, Category = GameType)
		bool OnlyHorizontal = false;
};



