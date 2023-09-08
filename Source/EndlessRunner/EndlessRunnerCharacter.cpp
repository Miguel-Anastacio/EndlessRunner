// Copyright Epic Games, Inc. All Rights Reserved.

#include "EndlessRunnerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// AEndlessRunnerCharacter

AEndlessRunnerCharacter::AEndlessRunnerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AEndlessRunnerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Bind function to Capsule component hit
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEndlessRunnerCharacter::OnCompHit);
}

void AEndlessRunnerCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);


	if (PlayerMovementState == WALLRUNING)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Wall run"));
		FHitResult Hit;
		if (ShootRayToWall(Hit))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Stuck To wall run"));
			UpdateWallRun();
		}
		else
		{
			EndWallRun();
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("Nothing"));
	}
}


void AEndlessRunnerCharacter::OnCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// test if player is falling
	if (GetCharacterMovement()->IsFalling())
	{
		if (PlayerMovementState == DEFAULT)
		{
			// find run direction and side
			FindRunDirectionAndSide(Hit.ImpactNormal);
			BeginWallRun();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEndlessRunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AEndlessRunnerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEndlessRunnerCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEndlessRunnerCharacter::Look);

	}

}

void AEndlessRunnerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AEndlessRunnerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AEndlessRunnerCharacter::Jump()
{
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
	if (JumpCurrentCount < JumpMaxCount)
		//LaunchCharacter(FindLaunchVelocity(), false, false);
	if (PlayerMovementState == WALLRUNING)
		EndWallRun();
}

void AEndlessRunnerCharacter::FindRunDirectionAndSide(FVector HitNormal)
{
	const FVector2D rightVector = FVector2D(GetActorRightVector().X, GetActorRightVector().Y);
	const FVector2D wallNormal2D = FVector2D(HitNormal.X, HitNormal.Y);
	// if the normal and the right vector of the player are pointing in the same direction then the wall is to the left
	if (UKismetMathLibrary::DotProduct2D(rightVector, wallNormal2D) > 0)
		WallPositionRelativeToPlayer = LEFT;
	else
		WallPositionRelativeToPlayer = RIGHT;
		
	switch (WallPositionRelativeToPlayer)
	{
	case RIGHT:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Right"));
		break;
	case LEFT:
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Left"));
		break;
	default:
		break;
	}

	FVector up;
	if (WallPositionRelativeToPlayer == LEFT)
	{
		up = FVector(0, 0, 1.0f);
		WallRunDirection = UKismetMathLibrary::Cross_VectorVector(HitNormal, up);
	}
	else if (WallPositionRelativeToPlayer == RIGHT)
	{
		up = FVector(0, 0, -1.0f);
		WallRunDirection = UKismetMathLibrary::Cross_VectorVector(HitNormal, up);
	}
}

void AEndlessRunnerCharacter::BeginWallRun()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Begin wall run"));
	UCharacterMovementComponent* playerCharacterMovement = GetCharacterMovement();
	playerCharacterMovement->GravityScale = 0.1;
	PlayerMovementState = WALLRUNING;
	playerCharacterMovement->AirControl = 1.0f;
	JumpCurrentCount = 0;
	playerCharacterMovement->MaxWalkSpeed = 1200;

}

void AEndlessRunnerCharacter::UpdateWallRun()
{
	/*
	if (!AreRequiredKeysDown())
	{
		EndWallRun();
		return;
	}
	WallSideENUM previousSide = WallSide;
	FindRunDirectionAndSide(Hit.ImpactNormal);
	// make sure side is the same
	// if it is different then end wall run
	if (previousSide != WallSide)
	{
		EndWallRun();
		return;
	}*/

	float maxSpeed = GetCharacterMovement()->GetMaxSpeed();
	FVector playerVelocity = FVector(WallRunDirection.X * maxSpeed, WallRunDirection.Y * maxSpeed, GetCharacterMovement()->Velocity.Z * 0);
	UE_LOG(LogTemp, Warning, TEXT("Velocity in X %f"), playerVelocity.X);
	UE_LOG(LogTemp, Warning, TEXT("Velocity in Y %f"), playerVelocity.Y);
	UE_LOG(LogTemp, Warning, TEXT("Velocity in Z %f"), playerVelocity.Z);

	GetCharacterMovement()->Velocity = playerVelocity;
}

void AEndlessRunnerCharacter::EndWallRun()
{
	UCharacterMovementComponent* playerCharacterMovement = GetCharacterMovement();
	playerCharacterMovement->GravityScale = 1;
	PlayerMovementState = DEFAULT;
	playerCharacterMovement->AirControl = 0.2f;
	playerCharacterMovement->MaxWalkSpeed = 800;

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("End wall run"));
}

bool AEndlessRunnerCharacter::ShootRayToWall(FHitResult& Hit)
{
	// get actor position
	FVector startRay = GetActorLocation();
	// get a vector from the actor to the wall
	FVector up;
	switch (WallPositionRelativeToPlayer)
	{
	case LEFT:
		up = FVector(0, 0, 1.0);
		break;
	case RIGHT:
		up = FVector(0, 0, -1.0);
		break;
	default:
		break;
	}
	FVector actorToWall = UKismetMathLibrary::Cross_VectorVector(WallRunDirection, up);
	actorToWall *= 200;
	// end position of ray
	FVector endRay = startRay + actorToWall;

	// You can use FCollisionQueryParams to further configure the query
	// Here we add ourselves to the ignored list so we won't block the trace
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Trace")), true, this);


	ECollisionChannel Channel = ECC_WorldStatic;

	// shoot a ray from the position of the actor to where the wall should be
	return GetWorld()->LineTraceSingleByChannel(Hit, startRay, endRay, Channel, TraceParams);
}




