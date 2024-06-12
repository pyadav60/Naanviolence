// Fill out your copyright notice in the Description page of Project Settings.


#include "NaanFighterGameMode.h"
#include "NaanFighterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANaanFighterGameMode::ANaanFighterGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/NaanFighter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Player Pawn class: %s"), *PlayerPawnBPClass.Class->GetName());

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Pawn was not found"));
	}
	// DefaultPawnClass = ANaanFighterCharacter::StaticClass();
}

