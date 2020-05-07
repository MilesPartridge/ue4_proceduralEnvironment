// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SynthComponent.h"
#include "DSP/Osc.h"
#include "MySynthComponent.generated.h"


// ========================================================================
// UMySynthComponent
// Synth component class which implements USynthComponent
// This is a simple hello-world type example which generates a sine-wave
// tone using a DSP oscillator class and implements a single function to set
// the frequency. 
// ========================================================================

UCLASS(ClassGroup = Synth, meta = (BlueprintSpawnableComponent))
class CTPRO870178_API UMySynthComponent : public USynthComponent
{
	GENERATED_BODY()
	
	// Called when synth is created
	virtual bool Init(int32& SampleRate) override;

	// Called to generate more audio
	virtual int32 OnGenerateAudio(float* OutAudio, int32 NumSamples) override;

public:
	// Sets the oscillator's frequency
	void SetFrequency(const float FrequencyHz = 440.0f);

	bool Tremolo = false;

protected:
	// A simple oscillator class. Can also generate Saw/Square/Tri/Noise.
	Audio::FOsc Osc, Osc2;

	int phase;
	int sampleRate;
};