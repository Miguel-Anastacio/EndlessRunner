// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
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
		UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere)
		float Velocity = 500.f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetVelocity(float velocity) { Velocity = velocity; };


	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void ReactToTrigger();


};
