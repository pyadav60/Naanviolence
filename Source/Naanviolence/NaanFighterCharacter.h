// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NaanFighterCharacter.generated.h"

UCLASS(config=Game)
class ANaanFighterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// different attacks
	void StartAttack1();
	void StartAttack2();
	void StartAttack3();
	void StartAttack4();


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ANaanFighterCharacter* otherPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	AActor* hurtbox;

	// damage the player
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float damageAmount);
	
	// has the player used basic attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool wasFirstAttackUsed;

	// the amount of health the character currently has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float playerHealth;

	// is the character's model flipped?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	bool isFlipped;

	// character's transform
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FTransform transform;

	// character's scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FVector scale;

public:
	// Sets default values for this character's properties
	ANaanFighterCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
