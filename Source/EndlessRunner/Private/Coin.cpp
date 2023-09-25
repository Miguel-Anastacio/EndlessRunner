// Fill out your copyright notice in the Description page of Project Settings.

#include "Coin.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
void ACoin::BeginPlay()
{
	Super::BeginPlay();

	//MagnetActivatedDelegate.AddUniqueDynamic(this, &ACoin::ActivateMagnet);
	Player= Cast<AEndlessRunnerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->MagnetActivatedDelegate.AddUniqueDynamic(this, &ACoin::ActivateMagnet);


	if (Player->GetIsMagnetActive())
	{
		ActivateMagnet(true);
	}

}

void ACoin::ReactToTrigger(AActor* OtherActor)
{
	AEndlessRunnerCharacter* player = Cast<AEndlessRunnerCharacter>(OtherActor);
	if (player)
	{
		player->IncreaseCoinsCollected();
		player->IncreaseScore(ScorePerCoin);
		if(AudioManager)
			AudioManager->CoinCollectedAudioComponent->Play();
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("AudioManager is null"));
		}
		Destroy();

	}
}

void ACoin::Tick(float deltaTime)
{
	if (!MagnetActive)
	{
		FVector currentLocation = GetActorLocation();
		currentLocation.X = currentLocation.X + Velocity * deltaTime;

		SetActorLocation(currentLocation);
		if (currentLocation.X > 3000.0f)
		{
			Destroy();
		}
	}
	else
	{
		Destination = Player->GetActorLocation();
		FVector movement = Destination - GetActorLocation();
		movement.Normalize();
		movement *= (Velocity * 3 * deltaTime);
		FVector currentLocation = GetActorLocation() + movement;
		SetActorLocation(currentLocation);
	}
}

void ACoin::ActivateMagnet(bool State)
{
	MagnetActive = State;
}
