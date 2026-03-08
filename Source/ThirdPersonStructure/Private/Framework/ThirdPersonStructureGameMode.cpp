// Copyright Epic Games, Inc. All Rights Reserved.

#include "Framework/ThirdPersonStructureGameMode.h"
#include "Player/ThirdPersonStructureCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdPersonStructureGameMode::AThirdPersonStructureGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
