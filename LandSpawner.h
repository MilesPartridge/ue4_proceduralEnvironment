// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandSpawner.generated.h"

UCLASS()
class CTPRO870178_API ALandSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


    int ifStatementTrigger_X, ifStatementTrigger_Y, ifStatementTrigger_NegativeX, ifStatementTrigger_NegativeY;

    //make this UPROPERTY editable in game or have some get_x function do it automatically in BeginPlay
    int squareSize = 3000;

    FVector coordinates = FVector(0, 0, 0);

    //                                     here is the object spawning stuff
    FVector objectSpawnCoordinates = FVector(0, 0, 0);
    FRotator objectSpawnRotation = FRotator(0, 0, 0);


    void spawnInObjects();

    int ObjectChooser;

    UPROPERTY(EditAnywhere, Category = "TheClassToSpawn")
        TSubclassOf<AActor> ActorToSpawn;
    UPROPERTY(EditAnywhere, Category = "TheClassToSpawn")
        TSubclassOf<AActor> ActorToSpawn2;
    UPROPERTY(EditAnywhere, Category = "TheClassToSpawn")
        TSubclassOf<AActor> ActorToSpawn3;

    int playerX, playerY, playerX_min, playerY_min, playerX_max, playerY_max;
    bool sideOfSquareToSpawnOn;

    //                      timer handling stuff
    void AdvanceTimer();

    void CountdownHasFinished();

    FTimerHandle CountdownTimerHandle;

    int CountdownTime = 2;
};
