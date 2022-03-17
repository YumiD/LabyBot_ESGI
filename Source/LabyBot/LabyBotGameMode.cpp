// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabyBotGameMode.h"
#include "LabyBotPawn.h"

ALabyBotGameMode::ALabyBotGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = ALabyBotPawn::StaticClass();
}