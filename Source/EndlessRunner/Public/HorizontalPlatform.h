// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnableObjects.h"
#include "HorizontalPlatform.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AHorizontalPlatform : public ASpawnableObjects
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
		float Length = 20.f;

public:
	//void BeginPlay() override;
};
