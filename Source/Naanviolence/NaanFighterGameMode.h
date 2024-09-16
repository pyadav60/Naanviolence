// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NaanFighterCharacter.h"
#include "NaanFighterGameMode.generated.h"

/**
 * 
 */
UCLASS(minimalapi)
class ANaanFighterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	ANaanFighterGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ANaanFighterCharacter* player1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ANaanFighterCharacter* player2;
};
