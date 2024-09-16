// Fill out your copyright notice in the Description page of Project Settings.


#include "NaanFighterCharacter.h"
#include "NaanFighterGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ANaanFighterCharacter::ANaanFighterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	//CameraBoom->bDoCollisionTest = false;
	//CameraBoom->TargetArmLength = 500.f;
	//CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	//CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	// Create a camera and attach to boom
	//SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	//SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // DO NOT Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;
	
	hurtbox = nullptr;
	otherPlayer = nullptr;
	characterState = ECharacterState::VE_Default;
	wasFirstAttackUsed = false;
	wasSecondAttackUsed = false;
	wasThirdAttackUsed = false;
	wasFourthAttackUsed = false;
	isFlipped = false;
	hasLandedHit = false;
	canMove = true;
	canAct = true;
	isDeviceForMultiplePlayers = false;
	playerHealth = 1.00f;
	transform = FTransform(FVector(0.0f, 0.0f, 0.0f));
	scale = FVector(1.0f, 1.0f, 1.0f);
	maxDistanceApart = 800.0f;
	isCrouching = false;
	stunTime = 0.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


 //	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

}

////// input

void ANaanFighterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// our casted NaanFighterGameMode gamemode always has the reference to both players
	// set gameMode to the casted gamemode, the GetWorld()->GetAuthGameMode() is builtin to unreal to get current gamemode
	if (auto gameMode = Cast<ANaanFighterGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (gameMode->player1 == this)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player 1 has bound their controls."));

			// set up gameplay key bindings
			PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ANaanFighterCharacter::Jump);
			PlayerInputComponent->BindAction("Jump", IE_Released, this, &ANaanFighterCharacter::StopJumping);
			PlayerInputComponent->BindAxis("MoveRight", this, &ANaanFighterCharacter::MoveRight);
			PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ANaanFighterCharacter::StartCrouching);
			PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ANaanFighterCharacter::StopCrouching);
			PlayerInputComponent->BindAction("Block", IE_Pressed, this, &ANaanFighterCharacter::StartBlocking);
			PlayerInputComponent->BindAction("Block", IE_Released, this, &ANaanFighterCharacter::StopBlocking);

			PlayerInputComponent->BindAction("Attack1", IE_Pressed, this, &ANaanFighterCharacter::StartAttack1);
			//PlayerInputComponent->BindAction("Attack1", IE_Released, this, &ANaanFighterCharacter::StopAttack1);
			PlayerInputComponent->BindAction("Attack2", IE_Pressed, this, &ANaanFighterCharacter::StartAttack2);
			//PlayerInputComponent->BindAction("Attack2", IE_Released, this, &ANaanFighterCharacter::StopAttack2);
			PlayerInputComponent->BindAction("Attack3", IE_Pressed, this, &ANaanFighterCharacter::StartAttack3);
			//PlayerInputComponent->BindAction("Attack3", IE_Released, this, &ANaanFighterCharacter::StopAttack3);
			PlayerInputComponent->BindAction("Attack4", IE_Pressed, this, &ANaanFighterCharacter::StartAttack4);
			//PlayerInputComponent->BindAction("Attack4", IE_Released, this, &ANaanFighterCharacter::StopAttack4);

			PlayerInputComponent->BindTouch(IE_Pressed, this, &ANaanFighterCharacter::TouchStarted);
			PlayerInputComponent->BindTouch(IE_Released, this, &ANaanFighterCharacter::TouchStopped);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player 2 has bound their controls."));

			// set up gameplay key bindings
			PlayerInputComponent->BindAction("JumpP2", IE_Pressed, this, &ACharacter::Jump);
			PlayerInputComponent->BindAction("JumpP2", IE_Released, this, &ACharacter::StopJumping);
			PlayerInputComponent->BindAxis("MoveRightP2", this, &ANaanFighterCharacter::MoveRight);
			PlayerInputComponent->BindAction("CrouchP2", IE_Pressed, this, &ANaanFighterCharacter::StartCrouching);
			PlayerInputComponent->BindAction("CrouchP2", IE_Released, this, &ANaanFighterCharacter::StopCrouching);
			PlayerInputComponent->BindAction("BlockP2", IE_Pressed, this, &ANaanFighterCharacter::StartBlocking);
			PlayerInputComponent->BindAction("BlockP2", IE_Released, this, &ANaanFighterCharacter::StopBlocking);

			PlayerInputComponent->BindAction("Attack1P2", IE_Pressed, this, &ANaanFighterCharacter::StartAttack1);
			//PlayerInputComponent->BindAction("Attack1P2", IE_Released, this, &ANaanFighterCharacter::StopAttack1);
			PlayerInputComponent->BindAction("Attack2P2", IE_Pressed, this, &ANaanFighterCharacter::StartAttack2);
			//PlayerInputComponent->BindAction("Attack2P2", IE_Released, this, &ANaanFighterCharacter::StopAttack2);
			PlayerInputComponent->BindAction("Attack3P2", IE_Pressed, this, &ANaanFighterCharacter::StartAttack3);
			//PlayerInputComponent->BindAction("Attack3P2", IE_Released, this, &ANaanFighterCharacter::StopAttack3);
			PlayerInputComponent->BindAction("Attack4P2", IE_Pressed, this, &ANaanFighterCharacter::StartAttack4);
			//PlayerInputComponent->BindAction("Attack4P2", IE_Released, this, &ANaanFighterCharacter::StopAttack4);

			PlayerInputComponent->BindTouch(IE_Pressed, this, &ANaanFighterCharacter::TouchStarted);
			PlayerInputComponent->BindTouch(IE_Released, this, &ANaanFighterCharacter::TouchStopped);
		}
	}
	

}

void ANaanFighterCharacter::Jump() 
{
	ACharacter::Jump();
	characterState = ECharacterState::VE_Jumping;
}

void ANaanFighterCharacter::StopJumping()
{
	ACharacter::StopJumping();
}

void ANaanFighterCharacter::Landed(const FHitResult& Hit)
{
	characterState = ECharacterState::VE_Default;
}


void ANaanFighterCharacter::StartCrouching()
{
	characterState = ECharacterState::VE_Crouching;
}

void ANaanFighterCharacter::StopCrouching()
{
	characterState = ECharacterState::VE_Default;
}

void ANaanFighterCharacter::StartBlocking()
{
	characterState = ECharacterState::VE_Blocking;
}

void ANaanFighterCharacter::StopBlocking()
{
	characterState = ECharacterState::VE_Default;
}

// value is taken in by default, to be between -1 and 1 depending on the input, 1 is moving right, -1 is left
void ANaanFighterCharacter::MoveRight(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("canAct is: %s"), canAct ? TEXT("true") : TEXT("false"));
	if (canAct && characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_Blocking) //add can move later
	{
		//UE_LOG(LogTemp, Warning, TEXT("The directional input is: %f"), Value);
		if (characterState != ECharacterState::VE_Jumping)
		{
			if (Value > 0.20f)
			{
				characterState = ECharacterState::VE_MovingRight;
			}
			else if (Value < -0.20f)
			{
				characterState = ECharacterState::VE_MovingLeft;
			}
			else
			{
				characterState = ECharacterState::VE_Default;
			}

			float currentDistanceApart = abs(otherPlayer->GetActorLocation().Y - GetActorLocation().Y);

			if (currentDistanceApart >= maxDistanceApart)
			{
				if ((currentDistanceApart + Value < currentDistanceApart && !isFlipped) || (currentDistanceApart - Value < currentDistanceApart && isFlipped))
				{
					// add movement in that direction, negative world direction because of the way y axis in inverted..
					AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
				}
			}
			else
			{
				// add movement in that direction, negative world direction because of the way y axis in inverted..
				AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
			}
		}
	}
}

void ANaanFighterCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ANaanFighterCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ANaanFighterCharacter::StartAttack1()
{
	UE_LOG(LogTemp, Warning, TEXT("Using attack 1"));
	wasFirstAttackUsed = true;
}

void ANaanFighterCharacter::StartAttack2()
{
	UE_LOG(LogTemp, Warning, TEXT("Using attack 2"));
	wasSecondAttackUsed = true;
}

void ANaanFighterCharacter::StartAttack3()
{
	UE_LOG(LogTemp, Warning, TEXT("Using attack 3"));
	wasThirdAttackUsed = true;
}

void ANaanFighterCharacter::StartAttack4()
{
	UE_LOG(LogTemp, Warning, TEXT("Using attack 4"));
	wasFourthAttackUsed = true;
}


// player 2 commands
void ANaanFighterCharacter::P2KeyboardAttack1()
{
	StartAttack1();
}

void ANaanFighterCharacter::P2KeyboardAttack2()
{
	StartAttack2();
}

void ANaanFighterCharacter::P2KeyboardAttack3()
{
	StartAttack3();
}

void ANaanFighterCharacter::P2KeyboardAttack4()
{
	StartAttack4();
}

void ANaanFighterCharacter::P2KeyboardJump()
{
	Jump();
}

void ANaanFighterCharacter::P2KeyboardStopJumping()
{
	StopJumping();
}

void ANaanFighterCharacter::P2KeyboardMoveRight(float Value)
{
	MoveRight(Value);
}


void ANaanFighterCharacter::CollideWithProximityHitbox()
{
	if ((characterState == ECharacterState::VE_MovingLeft && isFlipped) || (characterState == ECharacterState::VE_MovingRight && !isFlipped))
	{
		characterState = ECharacterState::VE_Blocking;
	}
}

void ANaanFighterCharacter::TakeDamage(float damageAmount, float hitstunTime, float blockstunTime)
{
	if (characterState != ECharacterState::VE_Blocking) {
		UE_LOG(LogTemp, Warning, TEXT("taking %f damage"), damageAmount);
		UE_LOG(LogTemp, Warning, TEXT("remaining %f"), playerHealth);
		playerHealth -= damageAmount;

		stunTime = hitstunTime;
		if (stunTime > 0.0f)
		{
			characterState = ECharacterState::VE_Stunned;
			BeginStun();
		}

		if (otherPlayer)
		{
			otherPlayer->hasLandedHit = true;
		}
	}
	else 
	{
		float reducedDamage = damageAmount * .2f;
		playerHealth -= reducedDamage;

		stunTime = blockstunTime;
		if (stunTime > 0.0f)
		{
			BeginStun();
		}
		else 
		{
			characterState = ECharacterState::VE_Default;
		}

	}

	if (playerHealth < 0.00f)
	{
		playerHealth = 0.00f;
	}
}

void ANaanFighterCharacter::BeginStun()
{
	canAct = false;
	GetWorld()->GetTimerManager().SetTimer(stunTimerHandle, this, &ANaanFighterCharacter::EndStun, stunTime, false);
}

void ANaanFighterCharacter::EndStun()
{
	characterState = ECharacterState::VE_Default;
	canAct = true;
}

//// Called when the game starts or when spawned
//void ANaanFighterCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//

// Called every frame
void ANaanFighterCharacter::Tick(float DeltaTime)
{
	// call super tick function
	Super::Tick(DeltaTime);

	if (characterState != ECharacterState::VE_Jumping)
	{
		// if otherPlayer is not a nullptr (means it has been assigned in the blueprints and exists)
		if (otherPlayer)
		{
			// NOTE: isFlipped is false when you are on the RIGHT (facing left), and true when you are on the LEFT (facing right)
			// UE_LOG(LogTemp, Warning, TEXT("%s"), isFlipped ? TEXT("True") : TEXT("False"));
			// try to automatically assign the CharacterMovement to our variable
			if (auto characterMovement = GetCharacterMovement())
			{
				// try to automatically assign the otherplayer CharacterMovement to our enemy variable
				if (auto enemyMovement = otherPlayer->GetCharacterMovement())
				{
					// if the enemy is to the right... (inverse Y coord is horizontal due to our camera setup, Z is vertical, X is near/far)
					if (enemyMovement->GetActorLocation().Y >= characterMovement->GetActorLocation().Y)
					{
						// and if we are facing left...
						if (isFlipped)
						{
							// we want to change the scale of the mesh, not the fighter BP (so we don't have to deal with other math)
							// temporarily hard coded, we get the child index 1 of CapsuleComponent of the fighter, which is the mesh
							// better practice instead to find component by name rather than hard code
							// better practice to make sure CapsuleComponent exists first like with the two autos above, but with those this is practically guaranteed
							if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
							{
								transform = mesh->GetRelativeTransform(); // gets transform box with location, rotation, scale and sets our temporary transform variable
								scale = transform.GetScale3D(); // gets the scale part
								scale.Y = -1;// mirror the mesh, flip to the right (characters face right by default, but note the inverted Y axis)
								transform.SetScale3D(scale); // update our transform variable with the new scale
								mesh->SetRelativeTransform(transform); // update the mesh's transform with the temporary transform variable
							}
							isFlipped = false;
						}
					}
					// if the enemy is to the left...
					else
					{
						// ... and if we are facing right
						if (!isFlipped)
						{
							// same comments as block above
							if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
							{
								transform = mesh->GetRelativeTransform(); // gets transform box with location, rotation, scale and sets our temporary transform variable
								scale = transform.GetScale3D(); // gets the scale part
								scale.Y = 1; // mirror the mesh, flip to the left (characters face right by default, but note the inverted Y axis)
								transform.SetScale3D(scale); // update our transform variable with the new scale
								mesh->SetRelativeTransform(transform); // update the mesh's transform with the temporary transform variable
							}
							isFlipped = true;
						}
					}
				}
			}
		}
	}
}

//
//// Called to bind functionality to input
//void ANaanFighterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

