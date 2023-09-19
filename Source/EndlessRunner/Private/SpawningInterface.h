// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnableObjects.h"
#include "SpawningInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpawningInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISpawningInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
	virtual ASpawnableObjects* SpawnObjectOnLocation(const FVector location, TSubclassOf<class ASpawnableObjects> objectToSpawn);
};
