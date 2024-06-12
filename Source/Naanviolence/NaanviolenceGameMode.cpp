// Copyright Epic Games, Inc. All Rights Reserved.

#include "NaanviolenceGameMode.h"
#include "NaanviolenceCharacter.h"
#include "NaanFighterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANaanviolenceGameMode::ANaanviolenceGameMode()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/NaanFighter"));
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		UE_LOG(LogTemp, Warning, TEXT("Your message here"));
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FAILED"));
	}*/

	// DefaultPawnClass = ANaanFighterCharacter::StaticClass();
}
