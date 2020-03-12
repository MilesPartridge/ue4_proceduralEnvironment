// Fill out your copyright notice in the Description page of Project Settings.


#include "LandSpawner.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"
#include "Math/Vector.h"
#include "Engine/World.h"

// Sets default values
ALandSpawner::ALandSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALandSpawner::BeginPlay()
{
	Super::BeginPlay();


    //                 set the if statement under tick up so it triggers in the right place
    FVector Location_Finder = FVector(0, 0, 0);
    Location_Finder = GetActorLocation();

    ifStatementTrigger_X = Location_Finder.X + (squareSize * 1.5); // as there are 3 squares, this one wants to move when the player is 1.5 squares ahead
    ifStatementTrigger_Y = Location_Finder.Y + (squareSize * 1.5);
    ifStatementTrigger_NegativeX = Location_Finder.X - (squareSize * 1.5);
    ifStatementTrigger_NegativeY = Location_Finder.Y - (squareSize * 1.5);


    coordinates = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(); // get player coordinates

    GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ALandSpawner::AdvanceTimer, 1.0f, true);
}

// Called every frame
void ALandSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    coordinates = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(); // get player coordinates



    if ((int)coordinates.X > ifStatementTrigger_X)
    {
        FVector newLocation = FVector(0, 0, 0);
        //this needs to be 2 squares along, and the half is because the point that gets moved is in the middle of the square
        //set the square from it's current location 2 squares ahead
        newLocation = GetActorLocation();
        newLocation.X += (squareSize * 3); // if the current location is 2 squares along, I still only want to add 2 squares

        SetActorLocation(newLocation, false);
        ifStatementTrigger_X += (squareSize * 3);
        ifStatementTrigger_NegativeX += (squareSize * 3);   // the negative version needs to 'keep up' with the player's movement across the squares
    }

    if ((int)coordinates.Y > ifStatementTrigger_Y)
    {
        FVector newLocation = FVector(0, 0, 0);
        //this needs to be 2 squares along, and the half is because the point that gets moved is in the middle of the square
        //set the square from it's current location 2 squares ahead
        newLocation = GetActorLocation();
        newLocation.Y += (squareSize * 3); // if the current location is 2 squares along, I still only want to add 2 squares

        SetActorLocation(newLocation, false);
        ifStatementTrigger_Y += (squareSize * 3);
        ifStatementTrigger_NegativeY += (squareSize * 3);
    }

    if ((int)coordinates.X < ifStatementTrigger_NegativeX)
    {
        FVector newLocation = FVector(0, 0, 0);
        //this needs to be 2 squares along, and the half is because the point that gets moved is in the middle of the square
        //set the square from it's current location 2 squares ahead
        newLocation = GetActorLocation();
        newLocation.X -= (squareSize * 3); // if the current location is 2 squares along, I still only want to add 2 squares

        SetActorLocation(newLocation, false);
        ifStatementTrigger_NegativeX -= (squareSize * 3);
        ifStatementTrigger_X -= (squareSize * 3);
    }

    if ((int)coordinates.Y < ifStatementTrigger_NegativeY)
    {
        FVector newLocation = FVector(0, 0, 0);
        //this needs to be 2 squares along, and the half is because the point that gets moved is in the middle of the square
        //set the square from it's current location 2 squares ahead
        newLocation = GetActorLocation();
        newLocation.Y -= (squareSize * 3); // if the current location is 2 squares along, I still only want to add 2 squares

        SetActorLocation(newLocation, false);
        ifStatementTrigger_NegativeY -= (squareSize * 3);
        ifStatementTrigger_Y -= (squareSize * 3);
    }
}


void ALandSpawner::spawnInObjects()
{
    UWorld* mainGame = GetWorld();

    playerX = coordinates.X;
    playerX_min = playerX - (squareSize * 1.5);
    playerX_max = playerX + (squareSize * 1.5);

    playerY = coordinates.Y;
    playerY_min = playerY - (squareSize * 1.5);
    playerY_max = playerY + (squareSize * 1.5);

    objectSpawnCoordinates = coordinates;


    sideOfSquareToSpawnOn = FMath::RandBool();


    //                          set side of square to spawn at
    if (sideOfSquareToSpawnOn) {

        sideOfSquareToSpawnOn = FMath::RandBool();
        //                          set opposite of square to spawn at on X
        if (sideOfSquareToSpawnOn) {
            objectSpawnCoordinates.X += (squareSize * 1.5);
        }
        else {
            objectSpawnCoordinates.X -= (squareSize * 1.5);
        }
        playerY = FMath::RandRange(playerY_min, playerY_max);
        objectSpawnCoordinates.Y = playerY;

    }
    else {

        sideOfSquareToSpawnOn = FMath::RandBool();
        //                          set opposite of square to spawn at on Y
        if (sideOfSquareToSpawnOn) {
            objectSpawnCoordinates.Y += (squareSize * 1.5);
        }
        else {
            objectSpawnCoordinates.Y -= (squareSize * 1.5);
        }
        playerX = FMath::RandRange(playerX_min, playerY_max);
        objectSpawnCoordinates.X = playerX;
    }



    // spawn in the shape

    ObjectChooser = FMath::RandRange(1, 3);

    switch (ObjectChooser) {
    case 1:
        mainGame->SpawnActor(ActorToSpawn, &objectSpawnCoordinates, &objectSpawnRotation);
        break;

    case 2:
        mainGame->SpawnActor(ActorToSpawn2, &objectSpawnCoordinates, &objectSpawnRotation);
        break;

    case 3:
        mainGame->SpawnActor(ActorToSpawn3, &objectSpawnCoordinates, &objectSpawnRotation);
        break;

    default:
        break;
    }

}



// the timer code was modified from official Unreal documentation at https://docs.unrealengine.com/en-US/Programming/Tutorials/VariablesTimersEvents/index.html
void ALandSpawner::AdvanceTimer()
{
    --CountdownTime;

    if (CountdownTime < 1)
    {
        //                  stop running the timer
        GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
        CountdownHasFinished();
    }
}

void ALandSpawner::CountdownHasFinished()
{
    spawnInObjects();
    CountdownTime = FMath::RandRange(1, 4);
    GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ALandSpawner::AdvanceTimer, 1.0f, true);
}
