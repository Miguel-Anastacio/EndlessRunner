// Fill out your copyright notice in the Description page of Project Settings.
#include "ObjectSpawner.h"
#include "HorizontalPlatform.h"

void AHorizontalPlatform::BeginPlay()
{
	Super::BeginPlay();
}

void AHorizontalPlatform::ReactToTrigger()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, TEXT("Overlap"));

	/*
	if(GetOwner() == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Owner is NULLLLLL"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, GetOwner()->GetName());
	}
	*/
	if (GetOwner() == NULL)
		return;

	AObjectSpawner* ObjectSpawner = Cast<AObjectSpawner>(GetOwner());
	if (!HasTriggeredSpawn)
	{
		if (ObjectSpawner)
		{
			if (!ObjectSpawner->GetIsFirstWall())
			{
				ObjectSpawner->SpawnWall();
				HasTriggeredSpawn = true;
			}

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Object Spawner is NULLLLLL"));
			HasTriggeredSpawn = true;
		}
	}
}
