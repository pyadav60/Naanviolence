// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitboxActor.generated.h"

UENUM(BlueprintType)
enum class EHitboxEnum : uint8
{
	HB_PROXIMITY UMETA(DisplayName = "Proximity"), // where the hitbox is and if it will hit
	HB_STRIKE    UMETA(DisplayName = "Strike"),    // what the hitbox hits on the opponent hurtbox, where, if its blocked, etc
	HB_HURTBOX   UMETA(DisplayName = "Hurtbox")    // what goes around the actual player
};


UCLASS()
class NAANVIOLENCE_API AHitboxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitboxActor();

	// update function, called every frame
	virtual void Tick(float DeltaTime) override;

	// notify the HitboxActorBP class instance that the hitbox is ready to be drawn
	UFUNCTION(BlueprintCallable)
	void TriggerVisualizeHitbox();

	// draw the hitbox to screen to visualize it (if this is a debug setting or practice mode)
	UFUNCTION(BlueprintImplementableEvent)
	void VisualizeHitbox();

	// the damage this hitbox will do
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	float hitboxDamage;

	// the hitbox enum instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	EHitboxEnum hitboxType;

	// the location to spawn hitbox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	FVector hitboxLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
