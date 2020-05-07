// Fill out your copyright notice in the Description page of Project Settings.


#include "MySynthComponent.h"



bool UMySynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;

	// Initialize the DSP objects
	Osc.Init(SampleRate);
	Osc.SetFrequency(220.0f);
	Osc.SetType(Audio::EOsc::Type::Square);

	Osc.Start();


	Osc2.Init(SampleRate);
	Osc2.SetFrequency(1.0f);
	Osc2.SetType(Audio::EOsc::Type::Sine);

	Osc2.Start();

	phase = 0;
	sampleRate = SampleRate;


	return true;
}

int32 UMySynthComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
{
	// Perform DSP operations here
	if (!Tremolo) {
		for (int32 Sample = 0; Sample < NumSamples; ++Sample)
		{
			OutAudio[Sample] = Osc.Generate() * 0.1;
		}
	}
	else {
		for (int32 Sample = 0; Sample < NumSamples; ++Sample)
		{
			OutAudio[Sample] = (Osc.Generate() * Osc2.Generate()) * 0.1;
		}
	}
	return NumSamples;
}

void UMySynthComponent::SetFrequency(const float InFrequencyHz)
{
	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
	SynthCommand([this, InFrequencyHz]()
	{
		Osc.SetFrequency(InFrequencyHz);
		Osc.Update();
	});
}