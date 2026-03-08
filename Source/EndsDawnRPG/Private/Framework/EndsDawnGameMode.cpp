// Copyright Epic Games, Inc. All Rights Reserved.

#include "Framework/EndsDawnGameMode.h"
#include "Player/EndsDawnCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEndsDawnGameMode::AEndsDawnGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
