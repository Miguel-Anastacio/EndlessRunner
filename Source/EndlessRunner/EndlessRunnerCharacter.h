// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "EndlessRunnerCharacter.generated.h"


UENUM(BlueprintType)
enum MovementState
{
	DEFAULT UMETA(DisplayName = "DEFAULT"),
	WALLRUNING UMETA(DisplayName = "WALLRUNNING")
};

UENUM(BlueprintType)
enum WallSide
{
	RIGHT UMETA(DisplayName = "RIGHT"),
	LEFT UMETA(DisplayName = "LEFT")
};


UCLASS(config=Game)
class AEndlessRunnerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SidewaysJumpAction;

public:
	AEndlessRunnerCharacter();
	

protected:
	bool IsPressed = false;


	//UPROPERTY(BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	MovementState PlayerMovementState = DEFAULT;

	//UPROPERTY(BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	WallSide WallPositionRelativeToPlayer = RIGHT;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector WallRunDirection = FVector(0, 0, 0);

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	void Jump();

	// mobile input
	FVector2D TouchPressedLocation;
	FVector2D TouchReleasedLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = JumpSettings, meta = (AllowPrivateAccess = "true"))
		float JumpThreshold = 20.f;
	// a swipe with a length over this value is a big jump
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = JumpSettings, meta = (AllowPrivateAccess = "true"))
		float JumpCutoff = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = JumpSettings, meta = (AllowPrivateAccess = "true"))
		float BigJump = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = JumpSettings, meta = (AllowPrivateAccess = "true"))
		float SmallJump = 0.5f;
	void TouchPressed(const FInputActionValue& Value);
	void TouchReleased(const FInputActionValue& Value);
	void TouchCanceled(const FInputActionValue& Value);
	void SidewaysJump(float& direction);

	// check if it is a swipe
	void CheckSwipe(FVector2D PressLocation, FVector2D ReleaseLocation, float SwipeThreshold);


	// wall run functions
	void FindRunDirectionAndSide(FVector HitNormal);
	void BeginWallRun();
	void UpdateWallRun();
	void EndWallRun();
	bool ShootRayToWall(FHitResult& Hit);

	FVector FindLaunchVelocity(float dir);
	void ClampHorizontalVelocity();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float deltaTime) override;

	// function bound to capsule component hit
	UFUNCTION() void OnCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

