// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"

void AObstacle::ReactToTrigger(AActor* OtherActor)
{
	AEndlessRunnerCharacter* player = Cast<AEndlessRunnerCharacter>(OtherActor);
	if (player)
	{
		if (player->GetAlive())
		{

			Impulse = Impulse + -player->GetCharacterMovement()->Velocity ;

			player->GetCharacterMovement()->AddImpulse(Impulse, true);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Impulse applied X %f"), Impulse.X));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Impulse applied Y %f"), Impulse.Y));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Impulse applied Z %f"), Impulse.Z));
			player->SetAlive(false);
			player->SetMovementState(AIRBORNE);

		}

	}
}

void AObstacle::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && (OtherActor != this))
	{
		if (Cast<APawn>(OtherActor) != NULL)
		{
			// if we are a obtacle that is on the right side of the track
			if (GetActorLocation().Y < 0)
			{
				// if player is on the left of the obstacle
				if (OtherActor->GetActorLocation().Y > GetActorLocation().Y)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Obstacle on the right of player"));
					Impulse = FVector(100, ImpulseAppliedToPlayer, 0);
				}
				else
				{
					Impulse = FVector(100, -ImpulseAppliedToPlayer, 0);
				}
				
			}
			else
			{
				// if player is on the right of the obstacle
				if (OtherActor->GetActorLocation().Y < GetActorLocation().Y)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Obstacle on the left of player"));

					Impulse = FVector(100, -ImpulseAppliedToPlayer, 0);
				}
				else
				{
					Impulse = FVector(100, +ImpulseAppliedToPlayer, 0);
				}
			}


		
			ReactToTrigger(OtherActor);
		}
	}

}

void AObstacle::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Impact Nomral X:  %f"), Hit.Normal.X));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Impact Nomral Y:  %f"), Hit.Normal.Y));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Impact Nomral Z:  %f"), Hit.Normal.Z));

	// see which is the prevalent direction from hit
	float max = FMath::Max3(abs(Hit.Normal.X), abs(Hit.Normal.Y), abs(Hit.Normal.Z));
	
	if (max == abs(Hit.Normal.X))
	{ 
		Impulse = FVector(0, ImpulseAppliedToPlayer, 0);
		ReactToTrigger(OtherActor);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Hit Straight"));
	}
	else if (max == abs(Hit.Normal.Y))
	{
		Impulse = ImpulseAppliedToPlayer * Hit.Normal;
		ReactToTrigger(OtherActor);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Hit from the side Obstacle"));
	}
	else
	{
		Impulse = FVector(0, 0, 0);
	}

}

void AObstacle::BeginPlay()
{
	Super::BeginPlay();
}

void AObstacle::TestCollisionDirection()
{
}
