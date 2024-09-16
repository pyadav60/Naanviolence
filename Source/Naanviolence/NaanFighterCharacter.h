// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NaanFighterCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	VE_Default UMETA(DisplayName = "NOT_MOVING"),
	VE_MovingRight UMETA(DisplayName = "MOVING_RIGHT"),
	VE_RunningRight UMETA(DisplayName = "RUNNING_RIGHT"),
	VE_MovingLeft UMETA(DisplayName = "MOVING_LEFT"),
	VE_RunningLeft UMETA(DisplayName = "RUNNING_LEFT"),
	VE_Jumping UMETA(DisplayName = "JUMPING"),
	VE_Stunned UMETA(DisplayName = "STUNNED"),
	VE_Blocking UMETA(DisplayName = "BLOCKING"),
	VE_Crouching UMETA(DisplayName = "CROUCHING")
};

UCLASS(config=Game)
class ANaanFighterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	// class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	// class USpringArmComponent* CameraBoom;

	// different attacks
	void StartAttack1();
	void StartAttack2();
	void StartAttack3();
	void StartAttack4();


	// these are all P2's controls. unreal doesn't like it when two objects of the same bp are assigned to same controller (for local multi).
	// so instead we mimic the player input component by making blueprint callable functions and execute logic in the code class and bp
	UFUNCTION(BlueprintCallable)
	void P2KeyboardAttack1();

	UFUNCTION(BlueprintCallable)
	void P2KeyboardAttack2();

	UFUNCTION(BlueprintCallable)
	void P2KeyboardAttack3();

	UFUNCTION(BlueprintCallable)
	void P2KeyboardAttack4();

	UFUNCTION(BlueprintCallable)
	void P2KeyboardJump();

	UFUNCTION(BlueprintCallable)
	void P2KeyboardStopJumping();

	UFUNCTION(BlueprintCallable)
	void P2KeyboardMoveRight(float Val);


protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	// Called when the game starts or when spawned
	// virtual void BeginPlay() override;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	// ECharacterClass characterClass;

	//Override ACharacter and APawn built in jump functionality
	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void Landed(const FHitResult& Hit) override;

	//Make character start crouch state
	UFUNCTION(BlueprintCallable)
	void StartCrouching();

	//Make character end crouch state
	UFUNCTION(BlueprintCallable)
	void StopCrouching();

	// pointer to the other player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ANaanFighterCharacter* otherPlayer;

	// a character's hurtbox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	AActor* hurtbox;

	// the direction the character is moving / the direction the player is holding down
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	ECharacterState characterState;

	// can the player move right now
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool canMove;

	// can the player move right now
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool canAct;

	//Determines the result of a player collision with any kind of proximity hitbox
	UFUNCTION(BlueprintCallable)
	void CollideWithProximityHitbox();

	// damage the player
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float damageAmount, float hitstunTime, float blockstunTime);
	
	//Cause the player to enter the stunned state
	void BeginStun();

	//Cause the player to leave the stunned state
	void EndStun();

	//Player Enters the blocking state
	UFUNCTION(BlueprintCallable)
	void StartBlocking();
	
	//Player Exits the Blocking State
	UFUNCTION(BlueprintCallable)
	void StopBlocking();

	// has the player used light attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool wasFirstAttackUsed;

	// has the player used medium attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool wasSecondAttackUsed;

	// has the player used heavy attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool wasThirdAttackUsed;

	// has the player used special attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool wasFourthAttackUsed;

	// has the player landed a hit with their last attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool hasLandedHit;

	// the amount of health the character currently has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float playerHealth;

	// the amount of health the character currently has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float maxDistanceApart;

	// is the character's model flipped?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	bool isFlipped;

	// character's transform
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FTransform transform;

	// character's scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FVector scale;

	// checks if we're using 2 inputs or same controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	bool isDeviceForMultiplePlayers;

	//Is the character crouching
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool isCrouching;

	//Amount of time the player character is disabled by hitstun, blockstun etc
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float stunTime;

	//Timer handle for managing stun duration
	FTimerHandle stunTimerHandle;

public:
	// Sets default values for this character's properties
	ANaanFighterCharacter();

	/** Returns SideViewCameraComponent subobject **/
	// FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	// FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
