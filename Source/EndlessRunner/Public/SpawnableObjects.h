// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnableObjects.generated.h"

UCLASS()
class ENDLESSRUNNER_API ASpawnableObjects : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnableObjects();

protected:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ObjectMesh;

	UPROPERTY(EditAnywhere)
		float Velocity = 500.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetVelocity(float velocity) { Velocity = velocity; };


};
