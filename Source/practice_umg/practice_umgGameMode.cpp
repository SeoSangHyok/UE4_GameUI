// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "practice_umgGameMode.h"
#include "practice_umgPlayerController.h"
#include "practice_umgCharacter.h"
#include "UObject/ConstructorHelpers.h"

Apractice_umgGameMode::Apractice_umgGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = Apractice_umgPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}