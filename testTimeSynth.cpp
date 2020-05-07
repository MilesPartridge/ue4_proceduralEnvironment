// Fill out your copyright notice in the Description page of Project Settings.


#include "testTimeSynth.h"

//#include "C:\UnrealEngine-release\Engine\Plugins\Runtime\TimeSynth\Source\TimeSynth\Classes\TimeSynthComponent.h"
#include "TimeSynthComponent.h"

// Sets default values
AtestTimeSynth::AtestTimeSynth()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	TimeSynth = CreateDefaultSubobject<UTimeSynthComponent>(TEXT("MusicTimeSynthComponent"));
}

// Called when the game starts or when spawned
void AtestTimeSynth::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AtestTimeSynth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

