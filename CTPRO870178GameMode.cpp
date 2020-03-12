// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "CTPRO870178GameMode.h"
#include "CTPRO870178Pawn.h"
#include "CTPRO870178Hud.h"

ACTPRO870178GameMode::ACTPRO870178GameMode()
{
	DefaultPawnClass = ACTPRO870178Pawn::StaticClass();
	HUDClass = ACTPRO870178Hud::StaticClass();
}
