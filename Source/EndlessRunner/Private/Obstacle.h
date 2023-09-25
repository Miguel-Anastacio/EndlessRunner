// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnableObjects.h"
#include "Obstacle.generated.h"

/**
 * 
 */
UCLASS()
class AObstacle : public ASpawnableObjects
{
	GENERATED_BODY()
public:
	AObstacle();

private:

	void ReactToTrigger(AActor* OtherActor) override;
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void BeginPlay() override;


protected:
	UPROPERTY(EditAnywhere, Category = Impulse)
		float ImpulseAppliedToPlayer = 1500.0f;
	FVector Impulse = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere)
		UBoxComponent* NoCoinArea = NULL;

	void TestCollisionDirection();

public:

};
