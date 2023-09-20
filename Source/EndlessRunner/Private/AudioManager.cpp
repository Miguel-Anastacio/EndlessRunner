// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioManager.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
// Sets default values
AAudioManager::AAudioManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BackgroundMusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Ambient Audio Component"));
	CoinCollectedAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Coin Collect Audio Component"));
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();

	BackgroundMusicAudioComponent->Play();
	CoinCollectedAudioComponent->Stop();
}


