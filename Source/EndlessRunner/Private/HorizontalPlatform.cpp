// Fill out your copyright notice in the Description page of Project Settings.
#include "ObjectSpawner.h"
#include "HorizontalPlatform.h"

void AHorizontalPlatform::BeginPlay()
{
	Super::BeginPlay();
}

void AHorizontalPlatform::ReactToTrigger()
{
	/*
	if(GetOwner() == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Owner is NULLLLLL"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, GetOwner()->GetName());
	}

	AObjectSpawner* ObjectSpawner = Cast<AObjectSpawner>(GetOwner());
	if (!HasTriggeredSpawn)
	{
		if (ObjectSpawner)
		{
			if (!ObjectSpawner->GetIsFirstWall())
			{
				//ObjectSpawner->SpawnWall();
				HasTriggeredSpawn = true;
			}

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Object Spawner is NULLLLLL"));
			HasTriggeredSpawn = true;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Already triggered spawn"));
	}*/
}
