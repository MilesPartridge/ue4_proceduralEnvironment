// Fill out your copyright notice in the Description page of Project Settings.


#include "LSystemSynth.h"

#include "LSystemSaveManager.h"
#include "Kismet/GameplayStatics.h"

#include "SynthComponents/EpicSynth1Component.h"
#include "EngineGlobals.h" // this is just for GEngine, as soon as printing to screen is not required, remove this
#include "UObject/ConstructorHelpers.h" // for initialising object pointers



// Sets default values
ALSystemSynth::ALSystemSynth()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	mySynth = CreateDefaultSubobject<UModularSynthComponent>(TEXT("synth"));
	mySynth->SetupAttachment(RootComponent);
	mySynth->bAutoActivate = true;


	static ConstructorHelpers::FObjectFinder<USoundCue> kick_01_Obj(TEXT("SoundCue'/Game/870178_Assets/AudioSamples/Kick01_Cue.Kick01_Cue'"));
	if (kick_01_Obj.Succeeded())
		kick_01 = kick_01_Obj.Object;

	Kick_01_Component = CreateDefaultSubobject<UAudioComponent>("KickAudioComponent");
	Kick_01_Component->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USoundCue> snare_01_Obj(TEXT("SoundCue'/Game/870178_Assets/AudioSamples/Snare01_Cue.Snare01_Cue'"));
	if (snare_01_Obj.Succeeded())
		snare_01 = snare_01_Obj.Object;

	Snare_01_Component = CreateDefaultSubobject<UAudioComponent>("SnareAudioComponent");
	Snare_01_Component->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USoundCue> hihatClosed_01_Obj(TEXT("SoundCue'/Game/870178_Assets/AudioSamples/HiHatClosed01_Cue.HiHatClosed01_Cue'"));
	if (hihatClosed_01_Obj.Succeeded())
		hihatClosed_01 = hihatClosed_01_Obj.Object;

	HihatClosed_01_Component = CreateDefaultSubobject<UAudioComponent>("HatAudioComponent");
	HihatClosed_01_Component->SetupAttachment(RootComponent);


	// setup chords
	MajorChords.push_back(std::vector<int>{0, 3, 8});
	MajorChords.push_back(std::vector<int>{0, 4, 7});
	MajorChords.push_back(std::vector<int>{0, 5, 9});
	MajorChords.push_back(std::vector<int>{1, 4, 9});
	MajorChords.push_back(std::vector<int>{1, 5, 8});
	MajorChords.push_back(std::vector<int>{1, 6, 10});
	MajorChords.push_back(std::vector<int>{2, 5, 10});
	MajorChords.push_back(std::vector<int>{2, 6, 9});
	MajorChords.push_back(std::vector<int>{2, 7, 11});
	MajorChords.push_back(std::vector<int>{3, 6, 11});
	MajorChords.push_back(std::vector<int>{3, 7, 10});
	MajorChords.push_back(std::vector<int>{4, 8, 11});

	MinorChords.push_back(std::vector<int>{0, 3, 7});
	MinorChords.push_back(std::vector<int>{0, 4, 9});
	MinorChords.push_back(std::vector<int>{0, 5, 8});
	MinorChords.push_back(std::vector<int>{1, 4, 8});
	MinorChords.push_back(std::vector<int>{1, 5, 10});
	MinorChords.push_back(std::vector<int>{1, 6, 9});
	MinorChords.push_back(std::vector<int>{2, 5, 9});
	MinorChords.push_back(std::vector<int>{2, 6, 11});
	MinorChords.push_back(std::vector<int>{2, 7, 10});
	MinorChords.push_back(std::vector<int>{3, 6, 10});
	MinorChords.push_back(std::vector<int>{3, 8, 11});
	MinorChords.push_back(std::vector<int>{4, 7, 11});

	AugmentedChords.push_back(std::vector<int>{0, 4, 8});
	AugmentedChords.push_back(std::vector<int>{1, 5, 9});
	AugmentedChords.push_back(std::vector<int>{2, 6, 10});
	AugmentedChords.push_back(std::vector<int>{3, 7, 11});

	DiminishedChords.push_back(std::vector<int>{0, 3, 6});
	DiminishedChords.push_back(std::vector<int>{0, 3, 9});
	DiminishedChords.push_back(std::vector<int>{0, 6, 9});
	DiminishedChords.push_back(std::vector<int>{1, 4, 7});
	DiminishedChords.push_back(std::vector<int>{1, 4, 10});
	DiminishedChords.push_back(std::vector<int>{1, 7, 10});
	DiminishedChords.push_back(std::vector<int>{2, 5, 8});
	DiminishedChords.push_back(std::vector<int>{2, 5, 11});
	DiminishedChords.push_back(std::vector<int>{2, 8, 11});
	DiminishedChords.push_back(std::vector<int>{3, 6, 9});
	DiminishedChords.push_back(std::vector<int>{4, 7, 10});
	DiminishedChords.push_back(std::vector<int>{5, 8, 11});
}

// Called when the game starts or when spawned
void ALSystemSynth::BeginPlay()
{
	Super::BeginPlay();

	mySynth->SetEnablePolyphony(true);
	mySynth->SetOscType(0, ESynth1OscType::Square);  

	i = 0;
	LSysNext = "";

	if (Kick_01_Component && kick_01)
	{
		Kick_01_Component->SetSound(kick_01);
	}
	if (Snare_01_Component && snare_01)
	{
		Snare_01_Component->SetSound(snare_01);
	}
	if (HihatClosed_01_Component && hihatClosed_01)
	{
		HihatClosed_01_Component->SetSound(hihatClosed_01);
	}

	for (int inti = 0; inti < DiminishedChords.size(); inti++)		//***** this prints to output log at the start to check chord arrays are correct. REMOVE LATER ************//
	{
		for (int j = 0; j < DiminishedChords[inti].size(); j++)
		{
			UE_LOG(LogTemp, Warning, TEXT("%d"), DiminishedChords[inti][j]);
		}
		UE_LOG(LogTemp, Warning, TEXT("next"));
	}
}

// Called every frame
void ALSystemSynth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALSystemSynth::TickLSystem()		// this L-System will sort the rhythm for the drums and notes
{
	//this print to screen code was found in gamedev.tv forums at: https://community.gamedev.tv/t/print-debug-messages-to-screen-from-c/4764
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,        // don't over write previous message, add a new one
			5.0f,   // Duration of message - limits distance messages scroll onto screen
			FColor::Cyan.WithAlpha(64),   // Color and transparancy!
			FString::Printf(TEXT("Rhythm        %s"), *LSysCurrent)  // Our usual text message format
			);
	}
//	c = LSysCurrent[i];

	while (i < LSysCurrent.Len() && LSysCurrent[i] != 'B')		// this still loops when there is a non-B char at the end
	{

		c = LSysCurrent[i];		// to avoid recopying this each iteration of the loop, make this a char* pointer to the ith point in the string

		switch (c)
		{
		case 'A':
			LSysNext += Rule_A;
			DrumsLSystem();
			break;
		case 'C':
			LSysNext += Rule_C;
			if (!playArpeggio)
			{
				NotesLSystem();

				UE_LOG(LogTemp, Warning, TEXT("CALLING NOTES"));
			}
			break;
		case 'D':
			LSysNext += Rule_D;
			if (!playArpeggio)
			{
				ModularLSystem();
			}
			break;
		case 'E':
			LSysNext += Rule_E;
			// this rule just sets arpeggio by adding +ACBACBACBACB- to the l system string
			break;
		case 'F':
			LSysNext += Rule_F;
			break;
		case 'G':
			LSysNext += Rule_G;
			break;
		case 'H':
			LSysNext += Rule_H;
			break;
		case '+':
			LSysNext += Rule_Plus;
			if (!playArpeggio)		//sets arpeggio playing
			{
				playArpeggio = true;
				mySynth->SetAttackTime(10);
				mySynth->SetReleaseTime(100);
				mySynth->SetEnablePolyphony(false);
				UE_LOG(LogTemp, Warning, TEXT("ARPEGGIO SET"));
			}
			break;
		case '-':
			LSysNext += Rule_Minus;
			if (playArpeggio)		//stops arpeggio playing
			{
				playArpeggio = false;
				mySynth->SetEnablePolyphony(true);
				UE_LOG(LogTemp, Warning, TEXT("ARPEGGIO UN-SET"));
			}
			break;
		}

		++i;
		
	}

	if (playArpeggio)
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYING NOTE"));
		switch (arpeggioCounter)
		{
		case 0:
			UE_LOG(LogTemp, Error, TEXT("case 0"));
			if (useMajorChords)
			{		//			<choose note in scale array>					<add triad to that number to make chord>
				mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + MajorChords[chordChooser][2], true, false);

				mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + MajorChords[chordChooser][0], FMath::RandRange(100, 127));
			}
			if (useMinorChords)
			{
				mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + MinorChords[chordChooser][2], true, false);

				mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + MinorChords[chordChooser][0], FMath::RandRange(100, 127));
			}
			if (useAugmentedChords)
			{
				mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + AugmentedChords[chordChooser][2], true, false);

				mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + AugmentedChords[chordChooser][0], FMath::RandRange(100, 127));
			}
			if (useDiminishedChords)
			{
				mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + DiminishedChords[chordChooser][2], true, false);

				mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + DiminishedChords[chordChooser][0], FMath::RandRange(100, 127));
			}
			break;
		case 1:
			UE_LOG(LogTemp, Error, TEXT("case 1"));
			if (useMajorChords)
			{		//			<choose note in scale array>					<add triad to that number to make chord>
				mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + MajorChords[chordChooser][0], true, false);

				mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + MajorChords[chordChooser][1], FMath::RandRange(100, 127));
			}
			if (useMinorChords)
			{
				mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + MinorChords[chordChooser][0], true, false);

				mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + MinorChords[chordChooser][1], FMath::RandRange(100, 127));
			}
			if (useAugmentedChords)
			{
				mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + AugmentedChords[chordChooser][0], true, false);

				mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + AugmentedChords[chordChooser][1], FMath::RandRange(100, 127));
			}
			if (useDiminishedChords)
			{
				mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + DiminishedChords[chordChooser][0], true, false);

				mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + DiminishedChords[chordChooser][1], FMath::RandRange(100, 127));
			}
			break;
		case 2:
			UE_LOG(LogTemp, Error, TEXT("case 2"));
			if (useMajorChords)
			{		//			<choose note in scale array>					<add triad to that number to make chord>
				mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + MajorChords[chordChooser][1], true, false);

				mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + MajorChords[chordChooser][2], FMath::RandRange(100, 127));
			}
			if (useMinorChords)
			{
				mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + MinorChords[chordChooser][1], true, false);

				mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + MinorChords[chordChooser][2], FMath::RandRange(100, 127));
			}
			if (useAugmentedChords)
			{
				mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + AugmentedChords[chordChooser][1], true, false);

				mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + AugmentedChords[chordChooser][2], FMath::RandRange(100, 127));
			}
			if (useDiminishedChords)
			{
				mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + DiminishedChords[chordChooser][1], true, false);

				mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + DiminishedChords[chordChooser][2], FMath::RandRange(100, 127));
			}
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("DEFAULT TRIGGERED !!!!!"));
			break;
		}
	}


	if (playArpeggio)
	{
		++arpeggioCounter;
		if (arpeggioCounter >= 3)
		{
			arpeggioCounter = 0;
		}
		UE_LOG(LogTemp, Warning, TEXT("ARPEGGIO COUNTER: %d"), arpeggioCounter);
	}

	++i;	// if this point is reached, it means that the while loop has either reached 'B' or has gone out of bounds. Either way, ++i (hence why next 'if' check is set up as is)

	if (reset)		// from the pause menu, the user can reset the L system with a new starting string
	{
		LSysCurrent = LSysResetAxiom;
		i = 0;
		octaveShifter = 0;
		currentNote = 0;
		LSysNext = "";
		generation = 0;
		playDrums = false;
		reset = false;
	}


	//at this point we have reached the current generation's maximum, and have fully grown the next gen. 
	//Start the next gen, reset + start process again.
	if (i >= LSysCurrent.Len()) {
		LSysCurrent = LSysNext;
		i = 0;
		++generation;
		LSysNext = "";
		if (generation >= 3)							// make the greater than value a modifiable variable	// this makes the drums kick in and out, alluding to musical structure
		{
			playDrums = true;
		}
		else if (generation >= 5)					// make the greater than value a modifiable variable // ************does this work? More testing
		{
			playDrums = false;
		}
		else if (generation >= 6)					// make the greater than value a modifiable variable
		{
			playDrums = true;
		}
	}

}

void ALSystemSynth::NotesLSystem()
{
	//this print to screen code was found in gamedev.tv forums at: https://community.gamedev.tv/t/print-debug-messages-to-screen-from-c/4764
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,        // don't over write previous message, add a new one
			5.0f,   // Duration of message - limits distance messages scroll onto screen
			FColor::Orange.WithAlpha(64),   // Color and transparancy!
			FString::Printf(TEXT("Notes        %s"), *NotesLSysCurrent)  // Our usual text message format
		);
	}

	//	c = LSysCurrent[i];

	while (Notesi < NotesLSysCurrent.Len() && NotesLSysCurrent[Notesi] != 'B')		// this still loops when there is a non-B char at the end
	{

		c = NotesLSysCurrent[Notesi];		// to avoid recopying this each iteration of the loop, make this a char* pointer to the ith point in the string

		switch (c)
		{
		case 'A':
			NotesLSysNext += NotesRule_A;
			UE_LOG(LogTemp, Error, TEXT("CHORD PLAYING"));
				if (useMajorChords)
				{		//			<choose note in scale array>					<add triad to that number to make chord>
					mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + MajorChords[chordChooser][0], FMath::RandRange(100, 127), FMath::RandRange(0.5, 2));
					mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + MajorChords[chordChooser][1], FMath::RandRange(100, 127), FMath::RandRange(0.5, 2));
					mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + MajorChords[chordChooser][2], FMath::RandRange(100, 127), FMath::RandRange(0.5, 2));
				}
				if (useMinorChords)
				{
					mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + MinorChords[chordChooser][0], FMath::RandRange(100, 127), FMath::RandRange(0.5, 2));
					mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + MinorChords[chordChooser][1], FMath::RandRange(100, 127), FMath::RandRange(0.5, 2));
					mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + MinorChords[chordChooser][2], FMath::RandRange(100, 127), FMath::RandRange(0.5, 2));
				}
				if (useAugmentedChords)
				{
					mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + AugmentedChords[chordChooser][0], FMath::RandRange(100, 127), FMath::RandRange(0.5, 2));
					mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + AugmentedChords[chordChooser][1], FMath::RandRange(100, 127), FMath::RandRange(0.5, 2));
					mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + AugmentedChords[chordChooser][2], FMath::RandRange(100, 127), FMath::RandRange(0.5, 2));
				}
				if (useDiminishedChords)
				{
					mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + DiminishedChords[chordChooser][0], FMath::RandRange(100, 127), FMath::RandRange(0.5, 2));
					mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + DiminishedChords[chordChooser][1], FMath::RandRange(100, 127), FMath::RandRange(0.5, 2));
					mySynth->NoteOn(notesToPlayCMajor[currentNote] + octaveShifter + DiminishedChords[chordChooser][2], FMath::RandRange(100, 127), FMath::RandRange(0.5, 2));
				}
			break;
		case 'C': 	// if an arpeggio is playing, it will play with the current chord, this stops any shifting of notes
				NotesLSysNext += NotesRule_C;
				currentNote += 5;
				if (currentNote > 6)
					currentNote -= 7;
			break;
		case 'D':
				NotesLSysNext += NotesRule_D;
				currentNote -= 3;
				if (currentNote < 0)
					currentNote += 7;
			break;
		case '+':
				NotesLSysNext += NotesRule_E;
				octaveShifter += 12;
				if (octaveShifter > 50)
					octaveShifter -= 12;
			break;
		case '-':
				NotesLSysNext += NotesRule_F;
				octaveShifter -= 12;
				if (octaveShifter < -12)
					octaveShifter += 12;
			break;
		case 'G':
			NotesLSysNext += NotesRule_G;
			//set major
			useMajorChords = true;
			useMinorChords = false;
			useAugmentedChords = false;
			useDiminishedChords = false;
			chordChooser = 0;
			maxNoChords = 12;
			break;
		case 'H':
			NotesLSysNext += NotesRule_H;
			//set minor
			useMinorChords = true;
			useMajorChords = false;
			useAugmentedChords = false;
			useDiminishedChords = false;
			chordChooser = 0;
			maxNoChords = 12;
			break;
		case 'I':
			NotesLSysNext += NotesRule_I;
			//set aug
			useAugmentedChords = true;
			useMajorChords = false;
			useMinorChords = false;
			useDiminishedChords = false;
			chordChooser = 0;
			maxNoChords = 4;
			break;
		case 'E':
			NotesLSysNext += NotesRule_Plus;
			//set dim
			useDiminishedChords = true;
			useMajorChords = false;
			useMinorChords = false;
			useAugmentedChords = false;
			chordChooser = 0;
			maxNoChords = 12;
			break;
		case 'F':
			NotesLSysNext += NotesRule_Minus;
			//move chord
			//choose randomly?
			chordChooser = FMath::RandRange(0, maxNoChords - 1);
			/*	or move through sequentially?
			++chordChooser;
			if (chordChooser >= maxNoChords)
			{
				chordChooser = 0;
			}
			*/
			break;
		case 'J':
			//				handling note offs
			
			NotesLSysNext += NotesRule_J;
			mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + MajorChords[chordChooser][0], true, false);
			
			break;

		}

		++Notesi;
	}


	++Notesi;	// if this point is reached, it means that the while loop has either reached 'B' or has gone out of bounds. Either way, ++i (hence why next 'if' check is set up as is)

	if (Notesreset)		// from the pause menu, the user can reset the L system with a new starting string
	{
		NotesLSysCurrent = NotesLSysResetAxiom;
		Notesi = 0;
		octaveShifter = 0;
		currentNote = 0;
		NotesLSysNext = "";
		Notesgeneration = 0;
		playDrums = false;
		Notesreset = false;
	}


	//at this point we have reached the current generation's maximum, and have fully grown the next gen. 
	//Start the next gen, reset + start process again.
	if (Notesi >= NotesLSysCurrent.Len()) {
		NotesLSysCurrent = NotesLSysNext;
		Notesi = 0;
		++Notesgeneration;
		NotesLSysNext = "";
		if (Notesgeneration >= 3)							// make the greater than value a modifiable variable	// this makes the drums kick in and out, alluding to musical structure
		{
			playDrums = true;
		}
		else if (Notesgeneration >= 5)					// make the greater than value a modifiable variable // ************does this work? More testing
		{
			playDrums = false;
		}
		else if (Notesgeneration >= 6)					// make the greater than value a modifiable variable
		{
			playDrums = true;
		}
	}
}

void ALSystemSynth::ModularLSystem()
{
	//this print to screen code was found in gamedev.tv forums at: https://community.gamedev.tv/t/print-debug-messages-to-screen-from-c/4764
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,        // don't over write previous message, add a new one
			5.0f,   // Duration of message - limits distance messages scroll onto screen
			FColor::White.WithAlpha(64),   // Color and transparancy!
			FString::Printf(TEXT("Modular        %s"), *ModularLSysCurrent)  // Our usual text message format
		);
	}

	//	c = LSysCurrent[i];

	while (Modulari < ModularLSysCurrent.Len() && ModularLSysCurrent[Modulari] != 'B')		// this still loops when there is a non-B char at the end
	{
		c = ModularLSysCurrent[Modulari];		// to avoid recopying this each iteration of the loop, make this a char* pointer to the ith point in the string

		switch (c)
		{
		case 'A':
			ModularLSysNext += ModularRule_A;
			attack = FMath::RandRange(10, 4000);
			mySynth->SetAttackTime(attack);	
			break;
		case 'C':
			ModularLSysNext += ModularRule_C;
			decay = FMath::RandRange(10, 1000);
			mySynth->SetDecayTime(decay);
			break;
		case 'D':
			ModularLSysNext += ModularRule_D;
			sustain = FMath::RandRange(0.01f, 1.0f);
			mySynth->SetSustainGain(sustain);
			break;
		case 'E':
			ModularLSysNext += ModularRule_E;
			release = FMath::RandRange(10, 4000);
			mySynth->SetReleaseTime(release);
			break;
		case 'F':
			//random waveform picker
			ModularLSysNext += ModularRule_F;
			waveForm = FMath::RandRange(0, 3);
			switch (waveForm)
			{
			case '0':
				mySynth->SetOscType(0, ESynth1OscType::Sine);
				break;
			case '1':
				mySynth->SetOscType(0, ESynth1OscType::Saw);
				break;
			case '2':
				mySynth->SetOscType(0, ESynth1OscType::Triangle);
				break;
			case '3':
				mySynth->SetOscType(0, ESynth1OscType::Square);
				break;
			}
			break;
		case 'G':
			ModularLSysNext += ModularRule_G;
			break;
		case 'H':
			ModularLSysNext += ModularRule_H;
			break;
		case 'I':
			ModularLSysNext += ModularRule_I;
			break;
		case 'J':
			ModularLSysNext += ModularRule_J;
			mySynth->NoteOff(notesToPlayCMajor[currentNote] + octaveShifter + MajorChords[chordChooser][0]);
			break;

		}

		++Modulari;

	}


	++Modulari;	// if this point is reached, it means that the while loop has either reached 'B' or has gone out of bounds. Either way, ++i (hence why next 'if' check is set up as is)

	if (Modularreset)		// from the pause menu, the user can reset the L system with a new starting string
	{
		ModularLSysCurrent = ModularLSysResetAxiom;
		Modulari = 0;
		octaveShifter = 0;
		currentNote = 0;
		ModularLSysNext = "";
		Modulargeneration = 0;
		playDrums = false;
		Modularreset = false;
	}


	//at this point we have reached the current generation's maximum, and have fully grown the next gen. 
	//Start the next gen, reset + start process again.
	if (Modulari >= ModularLSysCurrent.Len()) {
		ModularLSysCurrent = ModularLSysNext;
		Modulari = 0;
		++Modulargeneration;
		ModularLSysNext = "";
		if (Modulargeneration >= 3)							// make the greater than value a modifiable variable	// this makes the drums kick in and out, alluding to musical structure
		{
			playDrums = true;
		}
		else if (Modulargeneration >= 5)					// make the greater than value a modifiable variable // ************does this work? More testing
		{
			playDrums = false;
		}
		else if (Modulargeneration >= 6)					// make the greater than value a modifiable variable
		{
			playDrums = true;
		}
	}
}


void ALSystemSynth::DrumsLSystem()
{
	//this print to screen code was found in gamedev.tv forums at: https://community.gamedev.tv/t/print-debug-messages-to-screen-from-c/4764
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,        // don't over write previous message, add a new one
			5.0f,   // Duration of message - limits distance messages scroll onto screen
			FColor::Cyan.WithAlpha(64),   // Color and transparancy!
			FString::Printf(TEXT("Drums        %s"), *DrumsLSysCurrent)  // Our usual text message format
		);
	}

	//	c = LSysCurrent[i];

	while (Drumsi < DrumsLSysCurrent.Len() && DrumsLSysCurrent[Drumsi] != 'B')		// this still loops when there is a non-B char at the end
	{

		c = DrumsLSysCurrent[Drumsi];		// to avoid recopying this each iteration of the loop, make this a char* pointer to the ith point in the string

		switch (c)
		{
		case 'A':
			DrumsLSysNext += DrumsRule_A;
			if (Kick_01_Component && playDrums)
			{
				Kick_01_Component->Play(0.0f);
			}
			break;
		case 'C':
			DrumsLSysNext += DrumsRule_C;
			if (Snare_01_Component && playDrums)
			{
				Snare_01_Component->Play(0.0f);
			}
			break;
		case 'D':
			DrumsLSysNext += DrumsRule_D;
			if (HihatClosed_01_Component && playDrums)
			{
				HihatClosed_01_Component->Play(0.0f);
			}
			break;
		case 'E':
			DrumsLSysNext += DrumsRule_E;
			break;
		case 'F':
			DrumsLSysNext += DrumsRule_F;
			break;
		case 'G':
			DrumsLSysNext += DrumsRule_G;
			break;
		case 'H':
			DrumsLSysNext += DrumsRule_H;
			break;
		case '+':
			DrumsLSysNext += DrumsRule_Plus;
			break;
		case '-':
			DrumsLSysNext += DrumsRule_Minus;
			break;


		}

		++Drumsi;

	}


	++Drumsi;	// if this point is reached, it means that the while loop has either reached 'B' or has gone out of bounds. Either way, ++i (hence why next 'if' check is set up as is)

	if (Drumsreset)		// from the pause menu, the user can reset the L system with a new starting string
	{
		DrumsLSysCurrent = DrumsLSysResetAxiom;
		Drumsi = 0;
		octaveShifter = 0;
		currentNote = 0;
		DrumsLSysNext = "";
		Drumsgeneration = 0;
		playDrums = false;
		Drumsreset = false;
	}


	//at this point we have reached the current generation's maximum, and have fully grown the next gen. 
	//Start the next gen, reset + start process again.
	if (Drumsi >= DrumsLSysCurrent.Len()) {
		DrumsLSysCurrent = DrumsLSysNext;
		Drumsi = 0;
		++Drumsgeneration;
		DrumsLSysNext = "";
		if (Drumsgeneration >= 3)							// make the greater than value a modifiable variable	// this makes the drums kick in and out, alluding to musical structure
		{
			playDrums = true;
		}
		else if (Drumsgeneration >= 5)					// make the greater than value a modifiable variable // ************does this work? More testing
		{
			playDrums = false;
		}
		else if (Drumsgeneration >= 6)					// make the greater than value a modifiable variable
		{
			playDrums = true;
		}
	}
}



void ALSystemSynth::Set_Rule_A(FString ruleA)
{
	Rule_A = ruleA;
}

void ALSystemSynth::Set_Rule_C(FString ruleC)
{
	Rule_C = ruleC;
}

void ALSystemSynth::Set_Rule_D(FString ruleD)
{
	Rule_D = ruleD;
}

void ALSystemSynth::Set_Rule_E(FString ruleE)
{
	Rule_E = ruleE;
}

void ALSystemSynth::Set_Rule_F(FString ruleF)
{
	Rule_F = ruleF;
}

void ALSystemSynth::Set_Rule_G(FString ruleG)
{
	Rule_G = ruleG;
}

void ALSystemSynth::Set_Rule_H(FString ruleH)
{
	Rule_H = ruleH;
}

void ALSystemSynth::Set_Rule_Plus(FString rulePlus)
{
	Rule_Plus = rulePlus;
}

void ALSystemSynth::Set_Rule_Minus(FString ruleMinus)
{
	Rule_Minus = ruleMinus;
}

void ALSystemSynth::Set_New_Axiom(FString axiom)
{
	LSysResetAxiom = axiom;
	reset = true;
}






FString ALSystemSynth::Get_Rule_A()
{
	return Rule_A;
}

FString ALSystemSynth::Get_Rule_C()
{
	return Rule_C;
}

FString ALSystemSynth::Get_Rule_D()
{
	return Rule_D;
}

FString ALSystemSynth::Get_Rule_E()
{
	return Rule_E;
}

FString ALSystemSynth::Get_Rule_F()
{
	return Rule_F;
}

FString ALSystemSynth::Get_Rule_G()
{
	return Rule_G;
}

FString ALSystemSynth::Get_Rule_H()
{
	return Rule_H;
}

FString ALSystemSynth::Get_Rule_Plus()
{
	return Rule_Plus;
}

FString ALSystemSynth::Get_Rule_Minus()
{
	return Rule_Minus;
}

FString ALSystemSynth::Get_New_Axiom()
{
	return LSysResetAxiom;
	reset = true;
}

void ALSystemSynth::Save_Game()
{
	if (ULSystemSaveManager* SaveGameInstance = Cast<ULSystemSaveManager>(UGameplayStatics::CreateSaveGameObject(ULSystemSaveManager::StaticClass())))
	{
		// Set data on the savegame object.
		SaveGameInstance->Axiom = LSysResetAxiom;
		SaveGameInstance->Rule_A = Rule_A;
		SaveGameInstance->Rule_C = Rule_C;
		SaveGameInstance->Rule_D = Rule_D;
		SaveGameInstance->Rule_E = Rule_E;
		SaveGameInstance->Rule_F = Rule_F;
		SaveGameInstance->Rule_G = Rule_G;
		SaveGameInstance->Rule_H = Rule_H;
		SaveGameInstance->Rule_Plus = Rule_Plus;
		SaveGameInstance->Rule_Minus = Rule_Minus;


		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, "HelloWorld", 0))
		{
			// Save succeeded.
			UE_LOG(LogTemp, Warning, TEXT("Save Successful"));
		}
	}
}

void ALSystemSynth::Load_Game()
{
	ULSystemSaveManager* SaveGameInstance = Cast<ULSystemSaveManager>(UGameplayStatics::CreateSaveGameObject(ULSystemSaveManager::StaticClass()));

	SaveGameInstance = Cast<ULSystemSaveManager>(UGameplayStatics::LoadGameFromSlot("HelloWorld", 0));

	LSysResetAxiom = SaveGameInstance->Axiom;
	Rule_A = SaveGameInstance->Rule_A;
	Rule_C = SaveGameInstance->Rule_C;
	Rule_D = SaveGameInstance->Rule_D;
	Rule_E = SaveGameInstance->Rule_E;
	Rule_F = SaveGameInstance->Rule_F;
	Rule_G = SaveGameInstance->Rule_G;
	Rule_H = SaveGameInstance->Rule_H;
	Rule_Plus = SaveGameInstance->Rule_Plus;
	Rule_Minus = SaveGameInstance->Rule_Minus;

	reset = true;

	UE_LOG(LogTemp, Warning, TEXT("Load Complete"));
}

