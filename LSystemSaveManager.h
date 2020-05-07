// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LSystemSaveManager.generated.h"

/**
 * 
 */
UCLASS()
class CTPRO870178_API ULSystemSaveManager : public USaveGame
{
	GENERATED_BODY()

public:

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Axiom;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_A;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_C;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_D;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_E;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_F;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_G;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_H;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_Plus;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_Minus;



    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString SaveSlotName;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        uint32 UserIndex;

    ULSystemSaveManager();
};
