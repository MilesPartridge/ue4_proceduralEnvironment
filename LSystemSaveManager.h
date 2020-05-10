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
    //rhythm
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
        FString Rule_I;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_J;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_Plus;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_Minus;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_Branch;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString Rule_EndBranch;




    //Notes
    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesAxiom;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_A;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_C;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_D;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_E;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_F;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_G;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_H;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_I;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_J;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_Plus;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_Minus;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_Branch;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString NotesRule_EndBranch;




    //Drums
    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsAxiom;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_A;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_C;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_D;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_E;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_F;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_G;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_H;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_I;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_J;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_Plus;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_Minus;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_Branch;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString DrumsRule_EndBranch;




    //Modular
    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularAxiom;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_A;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_C;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_D;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_E;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_F;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_G;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_H;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_I;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_J;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_Plus;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_Minus;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_Branch;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString ModularRule_EndBranch;








    UPROPERTY(VisibleAnywhere, Category = Basic)
        FString SaveSlotName;

    UPROPERTY(VisibleAnywhere, Category = Basic)
        uint32 UserIndex;

    ULSystemSaveManager();
};
