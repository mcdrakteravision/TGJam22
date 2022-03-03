// Copyright Epic Games, Inc. All Rights Reserved.

#include "TGJamGameMode.h"
#include "TGJamPlayerController.h"
#include "TGJamCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATGJamGameMode::ATGJamGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATGJamPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}