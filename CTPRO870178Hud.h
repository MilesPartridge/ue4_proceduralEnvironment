// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "CTPRO870178Hud.generated.h"


UCLASS(config = Game)
class ACTPRO870178Hud : public AHUD
{
	GENERATED_BODY()

public:
	ACTPRO870178Hud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
