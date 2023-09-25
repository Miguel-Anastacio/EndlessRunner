// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectSpawner.h"
#include "EndlessRunner/EndlessRunnerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AObjectSpawner::AObjectSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HorizontalTransform = CreateDefaultSubobject<USceneComponent>("HorizontalSpawn");
	RootComponent = HorizontalTransform;
	WallRightTransform = CreateDefaultSubobject<USceneComponent>("RightWallSpawn");
	WallRightTransform->SetupAttachment(HorizontalTransform);
	WallLeftTransform = CreateDefaultSubobject<USceneComponent>("LeftWallSpawn");
	WallLeftTransform->SetupAttachment(HorizontalTransform);

}

float AObjectSpawner::CalculateVelocityOfWall()
{
	float aux = FMath::Clamp(timer, 0, TimeToReachMaxSpeed);

	float aux_range = TimeToReachMaxSpeed - 0;
	float range = MaxVelocity - MinVelocity;
	/*
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Aux %f"), aux));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Aux_range %f"), aux_range));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Range %f"), range));
	*/
	float velocity = ((aux - 0) / aux_range) * range + MinVelocity;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Wall Velocity %f"), velocity));
	AEndlessRunnerCharacter* player = Cast<AEndlessRunnerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Game speed % f"), velocity/100));
		player->SetGameSpeed(velocity / 100);
	}


	return velocity;
}

// Called when the game starts or when spawned
void AObjectSpawner::BeginPlay()
{
	Super::BeginPlay();
}

ASpawnableObjects* AObjectSpawner::SpawnObject()
{
	FTransform SpawnTransform;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnTransform.SetRotation(FQuat4d(0,0,0,1.0f));
	SpawnTransform.SetLocation(GetActorLocation());

	ASpawnableObjects* temp = nullptr;
	int index = 0;
	switch (CurrentWallSpawn)
	{
	case HORIZONTAL:
		SpawnTransform.SetLocation(HorizontalTransform->GetComponentLocation());
		index = FMath::RandRange(0, SpawnableHorizontalPlatforms.Num() -1);
		temp = GetWorld()->SpawnActor<AHorizontalPlatform>(SpawnableHorizontalPlatforms[index], SpawnTransform, SpawnParams);
		break;
	case WALL_LEFT:
		SpawnTransform.SetLocation(WallLeftTransform->GetComponentLocation());
		index = FMath::RandRange(0, SpawnableLeftVerticalPlatforms.Num() - 1);
		temp = GetWorld()->SpawnActor<AHorizontalPlatform>(SpawnableLeftVerticalPlatforms[index], SpawnTransform, SpawnParams);
		break;
	case WALL_RIGHT:
		SpawnTransform.SetLocation(WallRightTransform->GetComponentLocation());
		index = FMath::RandRange(0, SpawnableRightVerticalPlatforms.Num() - 1);
		temp = GetWorld()->SpawnActor<AHorizontalPlatform>(SpawnableRightVerticalPlatforms[index], SpawnTransform, SpawnParams);
		break;
	default:
		break;
	}
	
	
	return temp;
}

void AObjectSpawner::DecideWallToSpawn()
{
	switch (PreviousWallSpawn)
	{
	case HORIZONTAL:
		// if the player is on a horizontal platform
		// spawn vertical walls 1 or 2 in the enum WallSpawn
		CurrentWallSpawn = StaticCast<WallSpawn>(FMath::RandRange(0, 2));
		break;
	case WALL_LEFT:
		// if the player is on a left wall
		// spawn left vertical wall or horizontal 0 or 1 in the enum WallSpawn
		CurrentWallSpawn = StaticCast<WallSpawn>(FMath::RandRange(0, 1));
		break;
	case WALL_RIGHT:

		// if the player is on a right wall
		// spawn right vertical wall or horizontal 0 or 2 in the enum WallSpawn
		CurrentWallSpawn = StaticCast<WallSpawn>(FMath::RandRange(0, 2));
		while (CurrentWallSpawn == PreviousWallSpawn)
		{
			CurrentWallSpawn = StaticCast<WallSpawn>(FMath::RandRange(0, 2));
		}
		break;
	default:
		break;
	}
}

ASpawnableObjects* AObjectSpawner::SpawnObjectOnLocation(const FVector location, TSubclassOf<class ASpawnableObjects> objectToSpawn)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(location);
	if (objectToSpawn)
	{
		ASpawnableObjects* object = GetWorld()->SpawnActor<ASpawnableObjects>(objectToSpawn, SpawnTransform, SpawnParams);
		//AllObjects.Add(object);
		return object;
	}
	else
	{
		return NULL;
	}
}

void AObjectSpawner::SpawnWall()
{
	DecideWallToSpawn();
	ASpawnableObjects* object = SpawnObject();
	object->SetVelocity(CalculateVelocityOfWall());
	object->SetAudioManager(AudioManager);
	AllObjects.Add(object);

	//SpawnCoins();




	PreviousWallSpawn = CurrentWallSpawn;
}

// Called every frame
void AObjectSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsFirstWall && timer > SpawnCooldown)
	{
		SpawnWall();
		//timer = 0.0f;
		IsFirstWall = false;
	}
	timer += DeltaTime;
	
}




