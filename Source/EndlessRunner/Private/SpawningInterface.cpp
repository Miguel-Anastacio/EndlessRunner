// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawningInterface.h"

// Add default functionality here for any ISpawningInterface functions that are not pure virtual.

ASpawnableObjects* ISpawningInterface::SpawnObjectOnLocation(const FVector location, TSubclassOf<class ASpawnableObjects> objectToSpawn)
{
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(location);
	if (objectToSpawn)
	{
		//ASpawnableObjects* object = GetWorld()->SpawnActor<ASpawnableObjects>(objectToSpawn, SpawnTransform, SpawnParams);
		//object->SetVelocity(Velocity);
		//AllObjects.Add(object);
		return NULL;
	}
	else
	{
		return NULL;
	}
}
