// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SpawningInterface.h"
#include "EndlessRunnerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMagnetActivated, bool, State);

UENUM(BlueprintType)
enum MovementState
{
	DEFAULT UMETA(DisplayName = "DEFAULT"),
	WALLRUNING UMETA(DisplayName = "WALLRUNNING"),
	AIRBORNE UMETA(DisplayName = "AIRBORNE")
};

UENUM(BlueprintType)
enum WallSide
{
	RIGHT UMETA(DisplayName = "RIGHT"),
	LEFT UMETA(DisplayName = "LEFT")
};


UCLASS(config=Game)
class AEndlessRunnerCharacter : public ACharacter, public ISpawningInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext While on horizontal Platform */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** MappingContext while wall Running or in the air*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* AirborneMappingContext;

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

	// PowerUps Meshes
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MagnetPowerUpMesh;

	bool IsMagnetActive = false;
	float MagnetPowerUPDuration = 0.0f;
	float MagnetTimer = 0.0f;

protected:

	// debug
	float InputXAxis = 0.0f;
	float InputYAxis = 0.0f;


	bool IsPressed = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WallRunning)
		float RotationWhileWallRunning = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WallRunning)
		float RotationSpeedMultiplier = 20.0f;

	float CurrentXRotation = 0;
	float TargetXRotation = 0;
	float StartXRotation = 0;
	float RotationTimer = 0;
	
	UPROPERTY(BlueprintAssignable)
	FOnGameOverSignature GameOverDelegate;
	bool Alive = true;

	UPROPERTY(BlueprintReadOnly, Category = Score)
	float TotalScore = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = Score)
	float DistanceTravelled = 0.0f;

	// keeps track of the game speed used to calculate score based on disance travelled
	float GameSpeed = 150.0f;

	UPROPERTY(BlueprintReadOnly, Category = Score)
	int CoinsCollected = 0;


	UPROPERTY(BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<MovementState> PlayerMovementState = DEFAULT;

	//UPROPERTY(BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	WallSide WallPositionRelativeToPlayer = RIGHT;
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector WallRunDirection = FVector(0, 0, 0);

	// keeps track of phone gyroscope movement;
	FVector Tilt = FVector(0, 0, 0);
	FVector Gravity = FVector(0, 0, 0);
	FVector RotationRate = FVector(0, 0, 0);

	// stores the previous delta time
	// used in the move function to prevent input when it exceeds the boundaries 
	// only applied when the player is on an horziontal platform
	float tick = 0;


	//only capture tilt input if it is above this value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
		float TiltThreshold = 0.5f;;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	void Jump();

	// mobile input
	FVector2D TouchPressedLocation;
	FVector2D TouchReleasedLocation;
	// swipe has to be longer than threshold to trigger a jump
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = JumpSettings, meta = (AllowPrivateAccess = "true"))
	float JumpThreshold = 20.f;
	// a swipe with a length over this value is a big jump
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = JumpSettings, meta = (AllowPrivateAccess = "true"))
	float JumpCutoff = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = JumpSettings, meta = (AllowPrivateAccess = "true"))
	float BigJump = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = JumpSettings, meta = (AllowPrivateAccess = "true"))
	float SmallJump = 0.5f;

	// capture touch position at press and release
	void TouchPressed(const FInputActionValue& Value);
	void TouchReleased(const FInputActionValue& Value);

	void TouchCanceled(const FInputActionValue& Value);

	void SidewaysJump(float& direction);

	// check if it is a swipe
	// is a swipe if the distance between the pressloaction and the release location on the x axis 
	//is higher than the swipethreshold
	void CheckSwipe(FVector2D PressLocation, FVector2D ReleaseLocation, float SwipeThreshold);

	// wall run functions
	void FindRunDirectionAndSide(FVector HitNormal);
	void BeginWallRun();
	void UpdateWallRun();
	void EndWallRun();
	bool ShootRayToWall(FHitResult& Hit);

	void Landed(const FHitResult& Hit);

	FVector FindLaunchVelocity(float dir);
	void ClampHorizontalVelocity();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float deltaTime) override;

	// function bound to capsule component hit
	UFUNCTION() void OnCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);



public:
	AEndlessRunnerCharacter();

	FOnMagnetActivated MagnetActivatedDelegate;

	// function that inits all variables associated with the magnet powerup
	void InitMagnet(float duration, bool MagnetState);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FVector RelativeTransform;
	FRotator Rotation;


	// getters and seters
	void IncreaseCoinsCollected() {	CoinsCollected++; };
	void IncreaseScore(float amount);

	void SetGameSpeed(float gameSpeed);
	void SetMaxWalkSpeed(float speed, float minSpeed);

	MovementState GetMovementState() { return PlayerMovementState; };
	void SetMovementState(MovementState NewState);
	void SetMappingContext(UInputMappingContext* NewMappingContext);

	void SetAlive(bool status) { Alive = status; };
	bool GetAlive() { return Alive; };

	UStaticMeshComponent* GetMagnetMesh(); 
	void SetMagnetActive(bool state);
	bool GetIsMagnetActive();
	void SetMagnetPowerUpDuration(float duration);


	friend class AEndlessRunnerDebugHUD;


};

