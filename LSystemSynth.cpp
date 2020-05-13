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
	
	
	
	
	
	Scales.push_back(Major_Scale);
	Scales.push_back(Minor_Scale);
	Scales.push_back(Lydian_Scale);
	Scales.push_back(Mixolydian_Scale);
}

// Called when the game starts or when spawned
void ALSystemSynth::BeginPlay()
{
	Super::BeginPlay();
	
	Chords.push_back(MajorChords);
	Chords.push_back(MinorChords);
	Chords.push_back(AugmentedChords);
	Chords.push_back(DiminishedChords);

	mySynth->SetEnablePolyphony(true);
	mySynth->SetOscType(0, ESynth1OscType::Square);  
	mySynth->SetChorusEnabled(ChorusSetter);
	mySynth->SetEnableLegato(false);
	mySynth->SetEnableUnison(true);
	mySynth->SetEnableRetrigger(true);

	i = 0;
	LSysNext = "";

		//set up a pattern for chords to play (and branch to and from)
	branch.clear();
	for (int grow = 0; grow < growthSize; grow++)
	{						
							// this gives a one in 4 chance of being a note to play
		branch.push_back(FMath::RandRange(0, 3));		
	}
	stem.push_back(branch);

	// this is the number of branches produced before the song moves on to its next part
	treeSize = FMath::RandRange(3, 5); 


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

}

// Called every frame
void ALSystemSynth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALSystemSynth::TickLSystem()		// this L-System will sort the rhythm for the drums and notes
{
	//
//	c = LSysCurrent[i];



	while (i < LSysCurrent.Len() && LSysCurrent[i] != 'B')		// this still loops when there is a non-B char at the end
	{

		c = LSysCurrent[i];		// to avoid recopying this each iteration of the loop, make this a char* pointer to the ith point in the string

		switch (c)
		{
		case 'A':
			LSysNext += Rule_A;
			if (playDrums == true)
			{
				DrumsLSystem();//		bang drums every beat - their L-System runs in that manner
			}
			break;
		case 'C':
			LSysNext += Rule_C;
			if (!playArpeggio)// do not play chords while playing arpeggio
			{
				if (stem[stem.size()-1][beat] == 1)
				{
					NotesLSystem(playArpeggio);// calling this will always play a chord
				}
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
			// this rule just sets arpeggio
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
			}
			break;
		case '-':
			LSysNext += Rule_Minus;
			if (playArpeggio)		//stops arpeggio playing
			{
				playArpeggio = false;
				mySynth->SetEnablePolyphony(true);
				mySynth->SetAttackTime(FMath::RandRange(10, 2000));
				mySynth->SetReleaseTime(FMath::RandRange(10, 1000));
			}
			break;
		case '[':
			LSysNext += Rule_Branch;
			//handles branching
			branch.clear();
			for (int grow = 0; grow < growthSize; grow++)
			{
				branch.push_back(FMath::RandRange(0, 3));								// this gives a one in 4 chance of being a note to play
			}
			stem.push_back(branch);
			++branchesOnTheTree;
			if (branchesOnTheTree >= treeSize)
			{
				if (playDrums == false)
				{
					playDrums = true;
				}
				else {
					playDrums = false;
				}
				treeSize += FMath::RandRange(2, 5);
			}
			break;
		case ']':
			LSysNext += Rule_EndBranch;
			if (stem.size() > 1)
			{
				stem.pop_back();
			}
			break;
		}

		++i;

	}

	if (playArpeggio)
	{
		NotesLSystem(playArpeggio);
	}




	++i;	// if this point is reached, it means that the while loop has either reached 'B' or has gone out of bounds. Either way, ++i (hence why next 'if' check is set up as is)

	if (reset)		// from the pause menu, the user can reset the L system with a new starting string
	{
		LSysCurrent = LSysResetAxiom;
		i = 0;
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
	}
	//keep track of which beat we are on
	++beat;
	if (beat >= 15)
	{
		beat = 0;
		++bar;
		if (bar >= 300)
		{
			bar = 0;
		}
	}
}

void ALSystemSynth::NotesLSystem(bool arpeggio)
{

	//	c = LSysCurrent[i];
	if (!arpeggio)		// note the arpeggio completely disregards the L-System and plays the last played chord's notes
	{
		while (Notesi < NotesLSysCurrent.Len() && NotesLSysCurrent[Notesi] != 'B')		// this still loops when there is a non-B char at the end
		{

			c = NotesLSysCurrent[Notesi];		// to avoid recopying this each iteration of the loop, make this a char* pointer to the ith point in the string

			switch (c)
			{
			case 'A':
				// this cycles through available scales
				NotesLSysNext += NotesRule_A;

				if (scaleInUse == Major)
				{
					scaleInUse = Minor;
				}
				else if (scaleInUse == Minor)
				{
					scaleInUse = Lydian;
				}
				else if (scaleInUse == Lydian)
				{
					scaleInUse = Mixolydian;
				}
				else if (scaleInUse == Mixolydian)
				{
					scaleInUse = Major;
				}
				break;
			case 'C': 	// if an arpeggio is playing, it will play with the current chord, this stops any shifting of notes
				NotesLSysNext += NotesRule_C;
				noteOfScale += 5;
				if (noteOfScale > 6)
					noteOfScale -= 7;
				break;
			case 'D':
				NotesLSysNext += NotesRule_D;
				noteOfScale -= 3;
				if (noteOfScale < 0)
					noteOfScale += 7;
				break;
			case '+':
				NotesLSysNext += NotesRule_Plus;
				octaveShifter += 12;
				if (octaveShifter > 50)
					octaveShifter -= 12;
				break;
			case '-':
				NotesLSysNext += NotesRule_Minus;
				octaveShifter -= 12;
				if (octaveShifter < -12)
					octaveShifter += 12;
				break;
			case 'G':
				NotesLSysNext += NotesRule_G;
				//set major
				chordTypeInUse = Major;
				/*
				useMajorChords = true;
				useMinorChords = false;
				useAugmentedChords = false;
				useDiminishedChords = false;
				*/
				chordChooser = 0;
				maxNoChords = 12;
				break;
			case 'H':
				NotesLSysNext += NotesRule_H;
				//set minor
				chordTypeInUse = Minor;
				/*
				useMinorChords = true;
				useMajorChords = false;
				useAugmentedChords = false;
				useDiminishedChords = false;
				*/
				chordChooser = 0;
				maxNoChords = 12;
				break;
			case 'I':
				NotesLSysNext += NotesRule_I;
				//set aug
				chordTypeInUse = Augmented;
				/*
				useAugmentedChords = true;
				useMajorChords = false;
				useMinorChords = false;
				useDiminishedChords = false;
				*/
				chordChooser = 0;
				maxNoChords = 4;
				break;
			case 'J':
				NotesLSysNext += NotesRule_J;
				//set dim
				chordTypeInUse = Diminished;
				/*
				useDiminishedChords = true;
				useMajorChords = false;
				useMinorChords = false;
				useAugmentedChords = false;
				*/
				chordChooser = 0;
				maxNoChords = 12;
				break;
			case 'F':
				NotesLSysNext += NotesRule_F;
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
			case 'E':
				NotesLSysNext += NotesRule_E;
				//				move key in circle of fifths
				currentKey += 5;
				if (currentKey > 42)	// chosen 42 to keep C (36) in the middle
					currentKey -= 12;

				break;
			case '[':
				NotesLSysNext += NotesRule_Branch;
				if (noteStem.size() > 1)
				{
					++noteBranchIterator;
					while (noteStem.size() <= noteBranchIterator)
					{
						--noteBranchIterator;
					}
				}
				break;
			case ']':
				NotesLSysNext += NotesRule_EndBranch;
				if (noteStem.size() > 1)
				{
					noteStem.pop_back();
					while (noteStem.size() <= noteBranchIterator)
					{
						--noteBranchIterator;
					}
				}
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
			currentKey = 36;
			NotesLSysNext = "";
			Notesgeneration = 0;
			Notesreset = false;
			noteStem.clear();
			noteBranch.clear();
			triad.clear();
		}


		//at this point we have reached the current generation's maximum, and have fully grown the next gen. 
		//Start the next gen, reset + start process again.
		if (Notesi >= NotesLSysCurrent.Len()) {
			NotesLSysCurrent = NotesLSysNext;
			Notesi = 0;
			++Notesgeneration;
			NotesLSysNext = "";
		}
	}

	if (noteStem.size() < 1)		// if the stem is ready, loop. Keep looping until a branch is triggered
	{
		if (arpeggio)		// the arpeggio plays the last played chord's notes. This helps with congruency
		{
			switch (arpeggioCounter)
			{
			case 0:
				mySynth->NoteOff(1, true, false);
				mySynth->NoteOn(Scales[scaleInUse][noteOfScale] + currentKey + octaveShifter + Chords[chordTypeInUse][chordChooser][0], FMath::RandRange(100, 127));
				break;
			case 1:
				mySynth->NoteOff(1, true, false);
				mySynth->NoteOn(Scales[scaleInUse][noteOfScale] + currentKey + octaveShifter + Chords[chordTypeInUse][chordChooser][1], FMath::RandRange(100, 127));
				break;
			case 2:
				mySynth->NoteOff(1, true, false);
				mySynth->NoteOn(Scales[scaleInUse][noteOfScale] + currentKey + octaveShifter + Chords[chordTypeInUse][chordChooser][2], FMath::RandRange(100, 127));
				break;
			default:
				UE_LOG(LogTemp, Error, TEXT("DEFAULT TRIGGERED: ARPEGGIO !!!!!"));
				break;
			}

			++arpeggioCounter;
			if (arpeggioCounter >= 3)
			{
				arpeggioCounter = 0;
			}

		}
		else {		// if not arpeggio, then play a chord
			mySynth->NoteOff(1, true, false);
			mySynth->NoteOn(Scales[scaleInUse][noteOfScale] + currentKey + octaveShifter + Chords[chordTypeInUse][chordChooser][0], FMath::RandRange(100, 127), FMath::RandRange(0.5f, 2.0f));
			mySynth->NoteOn(Scales[scaleInUse][noteOfScale] + currentKey + octaveShifter + Chords[chordTypeInUse][chordChooser][1], FMath::RandRange(100, 127), FMath::RandRange(0.5f, 2.0f));
			mySynth->NoteOn(Scales[scaleInUse][noteOfScale] + currentKey + octaveShifter + Chords[chordTypeInUse][chordChooser][2], FMath::RandRange(100, 127), FMath::RandRange(0.5f, 2.0f));

			// all branch initialisation here
			triadNote1 = Scales[scaleInUse][noteOfScale] + currentKey + octaveShifter + Chords[chordTypeInUse][chordChooser][0];
			triadNote2 = Scales[scaleInUse][noteOfScale] + currentKey + octaveShifter + Chords[chordTypeInUse][chordChooser][1];
			triadNote3 = Scales[scaleInUse][noteOfScale] + currentKey + octaveShifter + Chords[chordTypeInUse][chordChooser][2];

			triad.push_back(triadNote1);
			triad.push_back(triadNote2);
			triad.push_back(triadNote3);

			noteBranch.push_back(triad);
			triad.clear();

			if (noteBranch.size() >= growthSize)
			{
				noteStem.push_back(noteBranch);
				noteBranch.clear();
			}
		}
	} else {
		if (arpeggio)		// the arpeggio plays the last played chord's notes. This helps with congruency
		{
			switch (arpeggioCounter)
			{
			case 0:
				mySynth->NoteOff(1, true, false);
				mySynth->NoteOn(noteStem[noteBranchIterator][noteTriadToPlay][0], FMath::RandRange(100, 127));
				break;
			case 1:
				mySynth->NoteOff(1, true, false);
				mySynth->NoteOn(noteStem[noteBranchIterator][noteTriadToPlay][1], FMath::RandRange(100, 127));
				break;
			case 2:
				mySynth->NoteOff(1, true, false);
				mySynth->NoteOn(noteStem[noteBranchIterator][noteTriadToPlay][2], FMath::RandRange(100, 127));
				break;
			default:
				UE_LOG(LogTemp, Error, TEXT("DEFAULT TRIGGERED: ARPEGGIO !!!!!"));
				break;
			}

			++arpeggioCounter;
			if (arpeggioCounter >= 3)
			{
				arpeggioCounter = 0;
			}

		}
		else {		// if not arpeggio, then play a chord
			mySynth->NoteOff(1, true, false);

			mySynth->NoteOn(noteStem[noteBranchIterator][noteTriadToPlay][0], FMath::RandRange(100, 127), FMath::RandRange(0.5f, 2.0f));
			mySynth->NoteOn(noteStem[noteBranchIterator][noteTriadToPlay][1], FMath::RandRange(100, 127), FMath::RandRange(0.5f, 2.0f));
			mySynth->NoteOn(noteStem[noteBranchIterator][noteTriadToPlay][2], FMath::RandRange(100, 127), FMath::RandRange(0.5f, 2.0f));

			++noteTriadToPlay;
			if (noteTriadToPlay >= growthSize)
			{
				noteTriadToPlay = 0;
			}

			// next branch initialisation here
			triadNote1 = Scales[scaleInUse][noteOfScale] + currentKey + octaveShifter + Chords[chordTypeInUse][chordChooser][0];
			triadNote2 = Scales[scaleInUse][noteOfScale] + currentKey + octaveShifter + Chords[chordTypeInUse][chordChooser][1];
			triadNote3 = Scales[scaleInUse][noteOfScale] + currentKey + octaveShifter + Chords[chordTypeInUse][chordChooser][2];

			triad.push_back(triadNote1);
			triad.push_back(triadNote2);
			triad.push_back(triadNote3);

			noteBranch.push_back(triad);
			triad.clear();

			if (noteBranch.size() >= growthSize)
			{
				noteStem.push_back(noteBranch);
				noteBranch.clear();
			}
		}
	}
}

void ALSystemSynth::ModularLSystem()
{
	

	//	c = LSysCurrent[i];

	while (Modulari < ModularLSysCurrent.Len() && ModularLSysCurrent[Modulari] != 'B')		// this still loops when there is a non-B char at the end
	{
		c = ModularLSysCurrent[Modulari];		// to avoid recopying this each iteration of the loop, make this a char* pointer to the ith point in the string

		switch (c)
		{
		case 'A':
			ModularLSysNext += ModularRule_A;
			attack = FMath::RandRange(10, 500);
			mySynth->SetAttackTime(attack);
			UE_LOG(LogTemp, Warning, TEXT("attack: %d"), attack);
			break;
		case 'C':
			ModularLSysNext += ModularRule_C;
			decay = FMath::RandRange(10, 600);
			mySynth->SetDecayTime(decay);
			UE_LOG(LogTemp, Warning, TEXT("decay: %d"), decay);
			break;
		case 'D':
			ModularLSysNext += ModularRule_D;
			sustain = FMath::RandRange(0.5f, 1.0f);
			mySynth->SetSustainGain(sustain);
			UE_LOG(LogTemp, Warning, TEXT("sus: %f"), sustain);
			break;
		case 'E':
			ModularLSysNext += ModularRule_E;
			release = FMath::RandRange(10, 600);
			mySynth->SetReleaseTime(release);
			UE_LOG(LogTemp, Warning, TEXT("release: %d"), release);
			break;
		case 'F':
			//random waveform picker
			ModularLSysNext += ModularRule_F;
			waveForm = FMath::RandRange(0, 3);
			switch (waveForm)
			{
			case 0:
				mySynth->SetOscType(0, ESynth1OscType::Sine);
				UE_LOG(LogTemp, Warning, TEXT("waveform: sine"));
				break;
			case 1:
				mySynth->SetOscType(0, ESynth1OscType::Saw);
				UE_LOG(LogTemp, Warning, TEXT("waveform: saw"));
				break;
			case 2:
				mySynth->SetOscType(0, ESynth1OscType::Triangle);
				UE_LOG(LogTemp, Warning, TEXT("waveform: tri"));
				break;
			case 3:
				mySynth->SetOscType(0, ESynth1OscType::Square);
				UE_LOG(LogTemp, Warning, TEXT("waveform: square"));
				break;
			}
			break;
		case 'G':
			ModularLSysNext += ModularRule_G;
			//filter randomiser
			filterFreq = FMath::RandRange(200, 16500);
			mySynth->SetFilterFrequency(filterFreq);
			UE_LOG(LogTemp, Error, TEXT("Filter Freq: %d"), filterFreq);
			break;
		case 'H':
			ModularLSysNext += ModularRule_H;
			// lfo patch randomiser
			switch (LFOpatchSelector)
			{
			case 0:
				mySynth->SetLFOPatch(0, ESynthLFOPatchType::PatchToFilterFreq);
				UE_LOG(LogTemp, Warning, TEXT("LFO: filter"));
				break;
			case 1:
		//		mySynth->SetLFOPatch(0, ESynthLFOPatchType::PatchToGain); // this was causing chaos
				UE_LOG(LogTemp, Warning, TEXT("LFO: gain"));
				break;
			case 2:
				mySynth->SetLFOPatch(0, ESynthLFOPatchType::PatchToOscFreq);
				UE_LOG(LogTemp, Warning, TEXT("LFO: pitch"));
				break;
			case 3:
				mySynth->SetLFOPatch(0, ESynthLFOPatchType::PatchToFilterQ);
				UE_LOG(LogTemp, Warning, TEXT("LFO: reso"));
				break;
			}
			++LFOpatchSelector;
			if (LFOpatchSelector > 3)
			{
				LFOpatchSelector = 0;
			}
			break;
		case 'I':
			ModularLSysNext += ModularRule_I;
			// chorus randomiser
			if (ChorusSetter == true)
			{
				ChorusSetter = false;
			} else {
				ChorusSetter = true;
			}
			mySynth->SetChorusEnabled(ChorusSetter);

			mySynth->SetChorusDepth(FMath::RandRange(0.0f, 0.7f));
			mySynth->SetChorusFeedback(FMath::RandRange(0.0f, 0.7f));
			mySynth->SetChorusFrequency(FMath::RandRange(0.0f, 1.5f));
			break;
		case 'J':
			ModularLSysNext += ModularRule_J;
			lfoGain = FMath::RandRange(0.2f, 0.8f);
			mySynth->SetLFOGain(0, lfoGain);
			break;
		case '+':
			ModularLSysNext += ModularRule_Plus;
			mySynth->SetLFOFrequency(0, FMath::RandRange(0.4f, 20.2f));
			break;
		case '-':
			ModularLSysNext += ModularRule_Minus;
			switch (LFOpatchSelector)
			{
			case 0:
				mySynth->SetLFOType(0, ESynthLFOType::Sine);
				break;
			case 1:
				mySynth->SetLFOType(0, ESynthLFOType::DownSaw);
				break;
			case 2:
				mySynth->SetLFOType(0, ESynthLFOType::RandomSampleHold);
				break;
			case 3:
				mySynth->SetLFOType(0, ESynthLFOType::Square);
				break;
			}
			++LFOpatchSelector;
			if (LFOpatchSelector > 3)
			{
				LFOpatchSelector = 0;
			}
			break;
		case '[':
			ModularLSysNext += ModularRule_Branch;
			//set delay
			if (DelaySetter == true)
			{
				DelaySetter = false;
			} else {
				DelaySetter = true;
			}
			mySynth->SetStereoDelayIsEnabled(DelaySetter);
			break;
		case ']':
			ModularLSysNext += ModularRule_EndBranch;
			//randomise all delay parameters
			mySynth->SetStereoDelayFeedback(FMath::RandRange(0.0f, 0.9f));
			mySynth->SetStereoDelayRatio(FMath::RandRange(0.01f, 0.99f));
			mySynth->SetStereoDelayTime(FMath::RandRange(10, 750));
			mySynth->SetStereoDelayWetlevel(FMath::RandRange(0.0f, 0.7f));

			switch (LFOpatchSelector)
			{
			case 0:
				mySynth->SetStereoDelayMode(ESynthStereoDelayMode::Count);
				break;
			case 1:
				mySynth->SetStereoDelayMode(ESynthStereoDelayMode::Cross);
				break;
			case 2:
				mySynth->SetStereoDelayMode(ESynthStereoDelayMode::Normal);
				break;
			case 3:
				mySynth->SetStereoDelayMode(ESynthStereoDelayMode::PingPong);
				break;
			}
			++LFOpatchSelector;		
			//using lfopatchselector in multiple parts adds randomness to all parts, but still will be the same each l-system
			if (LFOpatchSelector > 3)
			{
				LFOpatchSelector = 0;
			}
			break;
		}

		++Modulari;

	}


	++Modulari;	// if this point is reached, it means that the while loop has either reached 'B' or has gone out of bounds. Either way, ++i (hence why next 'if' check is set up as is)

	if (Modularreset)		// from the pause menu, the user can reset the L system with a new starting string
	{
		ModularLSysCurrent = ModularLSysResetAxiom;
		Modulari = 0;
		ModularLSysNext = "";
		Modulargeneration = 0;
		Modularreset = false;
	}


	//at this point we have reached the current generation's maximum, and have fully grown the next gen. 
	//Start the next gen, reset + start process again.
	if (Modulari >= ModularLSysCurrent.Len()) {
		ModularLSysCurrent = ModularLSysNext;
		Modulari = 0;
		++Modulargeneration;
		ModularLSysNext = "";
	}
}


void ALSystemSynth::DrumsLSystem()
{
	

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
			//this is used for drum fills
			break;
		case 'F':
			DrumsLSysNext += DrumsRule_F;
			//this is used for drum fills
			break;
		case 'G':
			DrumsLSysNext += DrumsRule_G;
			//randomly play kick or snare
			kickSnareRandomiser = FMath::RandRange(0, 1);
			if (kickSnareRandomiser)
			{
				if (Kick_01_Component && playDrums)
				{
					Kick_01_Component->Play(0.0f);
				}
			} else {
				if (Snare_01_Component && playDrums)
				{
					Snare_01_Component->Play(0.0f);
				}
			}
			break;
		case 'H':
			DrumsLSysNext += DrumsRule_H;
			//this is used for drum fills
			if (Kick_01_Component && playDrums)
			{
				Kick_01_Component->Play(0.0f);
			}
			break;
		case 'I':
			DrumsLSysNext += DrumsRule_I;
			//this is used for drum fills
			if (Snare_01_Component && playDrums)
			{
				Snare_01_Component->Play(0.0f);
			}
			break;
		case 'J':
			DrumsLSysNext += DrumsRule_J;
			break;
		case '+':
			DrumsLSysNext += DrumsRule_Plus;
			break;
		case '-':
			DrumsLSysNext += DrumsRule_Minus;
			break;

		case '[':
			DrumsLSysNext += DrumsRule_Branch;
			break;
		case ']':
			DrumsLSysNext += DrumsRule_EndBranch;
			break;

		}

		++Drumsi;

	}


	++Drumsi;	// if this point is reached, it means that the while loop has either reached 'B' or has gone out of bounds. Either way, ++i (hence why next 'if' check is set up as is)

	if (Drumsreset)		// from the pause menu, the user can reset the L system with a new starting string
	{
		DrumsLSysCurrent = DrumsLSysResetAxiom;
		Drumsi = 0;
		DrumsLSysNext = "";
		Drumsgeneration = 0;
		Drumsreset = false;
	}


	//at this point we have reached the current generation's maximum, and have fully grown the next gen. 
	//Start the next gen, reset + start process again.
	if (Drumsi >= DrumsLSysCurrent.Len()) {
		DrumsLSysCurrent = DrumsLSysNext;
		Drumsi = 0;
		++Drumsgeneration;
		DrumsLSysNext = "";
		if (Drumsgeneration == 12)							// make the greater than value a modifiable variable	// this makes the drums kick in and out, alluding to musical structure
		{
			DrumsLSysCurrent = DrumsRule_E;
		}
		else if (Drumsgeneration >= 16)					// make the greater than value a modifiable variable // ************does this work? More testing
		{
			DrumsLSysCurrent = DrumsRule_C;
			Drumsgeneration = 0;
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

void ALSystemSynth::Set_Rule_I(FString ruleI)
{
	Rule_I = ruleI;
}

void ALSystemSynth::Set_Rule_J(FString ruleJ)
{
	Rule_J = ruleJ;
}

void ALSystemSynth::Set_Rule_Plus(FString rulePlus)
{
	Rule_Plus = rulePlus;
}

void ALSystemSynth::Set_Rule_Minus(FString ruleMinus)
{
	Rule_Minus = ruleMinus;
}

void ALSystemSynth::Set_Rule_Branch(FString ruleBranch)
{
	Rule_Branch = ruleBranch;
}

void ALSystemSynth::Set_Rule_EndBranch(FString ruleEndBranch)
{
	Rule_EndBranch = ruleEndBranch;
}


// SETTING DRUM LSYSTEMS ////////////***


void ALSystemSynth::Set_Drum_Rule_A(FString Drum_ruleA)
{
	DrumsRule_A = Drum_ruleA;
}

void ALSystemSynth::Set_Drum_Rule_C(FString Drum_ruleC)
{
	DrumsRule_C = Drum_ruleC;
}

void ALSystemSynth::Set_Drum_Rule_D(FString Drum_ruleD)
{
	DrumsRule_D = Drum_ruleD;
}

void ALSystemSynth::Set_Drum_Rule_E(FString Drum_ruleE)
{
	DrumsRule_E = Drum_ruleE;
}

void ALSystemSynth::Set_Drum_Rule_F(FString Drum_ruleF)
{
	DrumsRule_F = Drum_ruleF;
}

void ALSystemSynth::Set_Drum_Rule_G(FString Drum_ruleG)
{
	DrumsRule_G = Drum_ruleG;
}

void ALSystemSynth::Set_Drum_Rule_H(FString Drum_ruleH)
{
	DrumsRule_H = Drum_ruleH;
}

void ALSystemSynth::Set_Drum_Rule_I(FString Drum_ruleI)
{
	DrumsRule_I = Drum_ruleI;
}

void ALSystemSynth::Set_Drum_Rule_J(FString Drum_ruleJ)
{
	DrumsRule_J = Drum_ruleJ;
}

void ALSystemSynth::Set_Drum_Rule_Plus(FString Drum_rulePlus)
{
	DrumsRule_Plus = Drum_rulePlus;
}

void ALSystemSynth::Set_Drum_Rule_Minus(FString Drum_ruleMinus)
{
	DrumsRule_Minus = Drum_ruleMinus;
}

void ALSystemSynth::Set_Drum_Rule_Branch(FString Drum_ruleBranch)
{
	DrumsRule_Branch = Drum_ruleBranch;
}

void ALSystemSynth::Set_Drum_Rule_EndBranch(FString Drum_ruleEndBranch)
{
	DrumsRule_EndBranch = Drum_ruleEndBranch;
}




// SETTING NOTE LSYSTEMS ////////////***


void ALSystemSynth::Set_Note_Rule_A(FString Note_ruleA)
{
	NotesRule_A = Note_ruleA;
}

void ALSystemSynth::Set_Note_Rule_C(FString Note_ruleC)
{
	NotesRule_C = Note_ruleC;
}

void ALSystemSynth::Set_Note_Rule_D(FString Note_ruleD)
{
	NotesRule_D = Note_ruleD;
}

void ALSystemSynth::Set_Note_Rule_E(FString Note_ruleE)
{
	NotesRule_E = Note_ruleE;
}

void ALSystemSynth::Set_Note_Rule_F(FString Note_ruleF)
{
	NotesRule_F = Note_ruleF;
}

void ALSystemSynth::Set_Note_Rule_G(FString Note_ruleG)
{
	NotesRule_G = Note_ruleG;
}

void ALSystemSynth::Set_Note_Rule_H(FString Note_ruleH)
{
	NotesRule_H = Note_ruleH;
}

void ALSystemSynth::Set_Note_Rule_I(FString Note_ruleI)
{
	NotesRule_I = Note_ruleI;
}

void ALSystemSynth::Set_Note_Rule_J(FString Note_ruleJ)
{
	NotesRule_J = Note_ruleJ;
}

void ALSystemSynth::Set_Note_Rule_Plus(FString Note_rulePlus)
{
	NotesRule_Plus = Note_rulePlus;
}

void ALSystemSynth::Set_Note_Rule_Minus(FString Note_ruleMinus)
{
	NotesRule_Minus = Note_ruleMinus;
}

void ALSystemSynth::Set_Note_Rule_Branch(FString Note_ruleBranch)
{
	NotesRule_Branch = Note_ruleBranch;
}

void ALSystemSynth::Set_Note_Rule_EndBranch(FString Note_ruleEndBranch)
{
	NotesRule_EndBranch = Note_ruleEndBranch;
}





// SETTING MODULAR LSYSTEMS ////////////***


void ALSystemSynth::Set_Modular_Rule_A(FString Modular_ruleA)
{
	ModularRule_A = Modular_ruleA;
}

void ALSystemSynth::Set_Modular_Rule_C(FString Modular_ruleC)
{
	ModularRule_C = Modular_ruleC;
}

void ALSystemSynth::Set_Modular_Rule_D(FString Modular_ruleD)
{
	ModularRule_D = Modular_ruleD;
}

void ALSystemSynth::Set_Modular_Rule_E(FString Modular_ruleE)
{
	ModularRule_E = Modular_ruleE;
}

void ALSystemSynth::Set_Modular_Rule_F(FString Modular_ruleF)
{
	ModularRule_F = Modular_ruleF;
}

void ALSystemSynth::Set_Modular_Rule_G(FString Modular_ruleG)
{
	ModularRule_G = Modular_ruleG;
}

void ALSystemSynth::Set_Modular_Rule_H(FString Modular_ruleH)
{
	ModularRule_H = Modular_ruleH;
}

void ALSystemSynth::Set_Modular_Rule_I(FString Modular_ruleI)
{
	ModularRule_I = Modular_ruleI;
}

void ALSystemSynth::Set_Modular_Rule_J(FString Modular_ruleJ)
{
	ModularRule_J = Modular_ruleJ;
}

void ALSystemSynth::Set_Modular_Rule_Plus(FString Modular_rulePlus)
{
	ModularRule_Plus = Modular_rulePlus;
}

void ALSystemSynth::Set_Modular_Rule_Minus(FString Modular_ruleMinus)
{
	ModularRule_Minus = Modular_ruleMinus;
}

void ALSystemSynth::Set_Modular_Rule_Branch(FString Modular_ruleBranch)
{
	ModularRule_Branch = Modular_ruleBranch;
}

void ALSystemSynth::Set_Modular_Rule_EndBranch(FString Modular_ruleEndBranch)
{
	ModularRule_EndBranch = Modular_ruleEndBranch;
}




// SET RESET AXIOM ///////********

void ALSystemSynth::Set_New_Axiom(FString axiom)
{
	LSysResetAxiom = axiom;
	reset = true;
	Drumsreset = true;
	Notesreset = true;
	Modularreset = true;
}


void ALSystemSynth::Set_New_Drum_Axiom(FString axiom)
{
	DrumsLSysResetAxiom = axiom;
	reset = true;
	Drumsreset = true;
	Notesreset = true;
	Modularreset = true;
}


void ALSystemSynth::Set_New_Notes_Axiom(FString axiom)
{
	NotesLSysResetAxiom = axiom;
	reset = true;
	Drumsreset = true;
	Notesreset = true;
	Modularreset = true;
}


void ALSystemSynth::Set_New_Modular_Axiom(FString axiom)
{
	ModularLSysCurrent = axiom;
	reset = true;
	Drumsreset = true;
	Notesreset = true;
	Modularreset = true;
}




// create getters


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

FString ALSystemSynth::Get_Rule_I()
{
	return Rule_I;
}

FString ALSystemSynth::Get_Rule_J()
{
	return Rule_J;
}

FString ALSystemSynth::Get_Rule_Plus()
{
	return Rule_Plus;
}

FString ALSystemSynth::Get_Rule_Minus()
{
	return Rule_Minus;
}

FString ALSystemSynth::Get_Rule_Branch()
{
	return Rule_Branch;
}

FString ALSystemSynth::Get_Rule_EndBranch()
{
	return Rule_EndBranch;
}





FString ALSystemSynth::Get_NotesRule_A()
{
	return NotesRule_A;
}

FString ALSystemSynth::Get_NotesRule_C()
{
	return NotesRule_C;
}

FString ALSystemSynth::Get_NotesRule_D()
{
	return NotesRule_D;
}

FString ALSystemSynth::Get_NotesRule_E()
{
	return NotesRule_E;
}

FString ALSystemSynth::Get_NotesRule_F()
{
	return NotesRule_F;
}

FString ALSystemSynth::Get_NotesRule_G()
{
	return NotesRule_G;
}

FString ALSystemSynth::Get_NotesRule_H()
{
	return NotesRule_H;
}

FString ALSystemSynth::Get_NotesRule_I()
{
	return NotesRule_I;
}

FString ALSystemSynth::Get_NotesRule_J()
{
	return NotesRule_J;
}

FString ALSystemSynth::Get_NotesRule_Plus()
{
	return NotesRule_Plus;
}

FString ALSystemSynth::Get_NotesRule_Minus()
{
	return NotesRule_Minus;
}

FString ALSystemSynth::Get_NotesRule_Branch()
{
	return NotesRule_Branch;
}

FString ALSystemSynth::Get_NotesRule_EndBranch()
{
	return NotesRule_EndBranch;
}





FString ALSystemSynth::Get_ModularRule_A()
{
	return ModularRule_A;
}

FString ALSystemSynth::Get_ModularRule_C()
{
	return ModularRule_C;
}

FString ALSystemSynth::Get_ModularRule_D()
{
	return ModularRule_D;
}

FString ALSystemSynth::Get_ModularRule_E()
{
	return ModularRule_E;
}

FString ALSystemSynth::Get_ModularRule_F()
{
	return ModularRule_F;
}

FString ALSystemSynth::Get_ModularRule_G()
{
	return ModularRule_G;
}

FString ALSystemSynth::Get_ModularRule_H()
{
	return ModularRule_H;
}

FString ALSystemSynth::Get_ModularRule_I()
{
	return ModularRule_I;
}

FString ALSystemSynth::Get_ModularRule_J()
{
	return ModularRule_J;
}

FString ALSystemSynth::Get_ModularRule_Plus()
{
	return ModularRule_Plus;
}

FString ALSystemSynth::Get_ModularRule_Minus()
{
	return ModularRule_Minus;
}

FString ALSystemSynth::Get_ModularRule_Branch()
{
	return ModularRule_Branch;
}

FString ALSystemSynth::Get_ModularRule_EndBranch()
{
	return ModularRule_EndBranch;
}






FString ALSystemSynth::Get_DrumsRule_A()
{
	return DrumsRule_A;
}

FString ALSystemSynth::Get_DrumsRule_C()
{
	return DrumsRule_C;
}

FString ALSystemSynth::Get_DrumsRule_D()
{
	return DrumsRule_D;
}

FString ALSystemSynth::Get_DrumsRule_E()
{
	return DrumsRule_E;
}

FString ALSystemSynth::Get_DrumsRule_F()
{
	return DrumsRule_F;
}

FString ALSystemSynth::Get_DrumsRule_G()
{
	return DrumsRule_G;
}

FString ALSystemSynth::Get_DrumsRule_H()
{
	return DrumsRule_H;
}

FString ALSystemSynth::Get_DrumsRule_I()
{
	return DrumsRule_I;
}

FString ALSystemSynth::Get_DrumsRule_J()
{
	return DrumsRule_J;
}

FString ALSystemSynth::Get_DrumsRule_Plus()
{
	return DrumsRule_Plus;
}

FString ALSystemSynth::Get_DrumsRule_Minus()
{
	return DrumsRule_Minus;
}

FString ALSystemSynth::Get_DrumsRule_Branch()
{
	return DrumsRule_Branch;
}

FString ALSystemSynth::Get_DrumsRule_EndBranch()
{
	return DrumsRule_EndBranch;
}







void ALSystemSynth::Save_Game(FString SaveSlotName)
{
	if (ULSystemSaveManager* SaveGameInstance = Cast<ULSystemSaveManager>(UGameplayStatics::CreateSaveGameObject(ULSystemSaveManager::StaticClass())))
	{
		// Set data on the savegame object.

		//rhythm system
		SaveGameInstance->Axiom = LSysResetAxiom;
		SaveGameInstance->Rule_A = Rule_A;
		SaveGameInstance->Rule_C = Rule_C;
		SaveGameInstance->Rule_D = Rule_D;
		SaveGameInstance->Rule_E = Rule_E;
		SaveGameInstance->Rule_F = Rule_F;
		SaveGameInstance->Rule_G = Rule_G;
		SaveGameInstance->Rule_H = Rule_H;
		SaveGameInstance->Rule_I = Rule_I;
		SaveGameInstance->Rule_J = Rule_J;
		SaveGameInstance->Rule_Plus = Rule_Plus;
		SaveGameInstance->Rule_Minus = Rule_Minus;
		SaveGameInstance->Rule_Branch = Rule_Branch;
		SaveGameInstance->Rule_EndBranch = Rule_EndBranch;

		//notes system
		SaveGameInstance->NotesAxiom = NotesLSysResetAxiom;
		SaveGameInstance->NotesRule_A = NotesRule_A;
		SaveGameInstance->NotesRule_C = NotesRule_C;
		SaveGameInstance->NotesRule_D = NotesRule_D;
		SaveGameInstance->NotesRule_E = NotesRule_E;
		SaveGameInstance->NotesRule_F = NotesRule_F;
		SaveGameInstance->NotesRule_G = NotesRule_G;
		SaveGameInstance->NotesRule_H = NotesRule_H;
		SaveGameInstance->NotesRule_I = NotesRule_I;
		SaveGameInstance->NotesRule_J = NotesRule_J;
		SaveGameInstance->NotesRule_Plus = NotesRule_Plus;
		SaveGameInstance->NotesRule_Minus = NotesRule_Minus;
		SaveGameInstance->NotesRule_Branch = NotesRule_Branch;
		SaveGameInstance->NotesRule_EndBranch = NotesRule_EndBranch;

		//Drums system
		SaveGameInstance->DrumsAxiom = DrumsLSysResetAxiom;
		SaveGameInstance->DrumsRule_A = DrumsRule_A;
		SaveGameInstance->DrumsRule_C = DrumsRule_C;
		SaveGameInstance->DrumsRule_D = DrumsRule_D;
		SaveGameInstance->DrumsRule_E = DrumsRule_E;
		SaveGameInstance->DrumsRule_F = DrumsRule_F;
		SaveGameInstance->DrumsRule_G = DrumsRule_G;
		SaveGameInstance->DrumsRule_H = DrumsRule_H;
		SaveGameInstance->DrumsRule_I = DrumsRule_I;
		SaveGameInstance->DrumsRule_J = DrumsRule_J;
		SaveGameInstance->DrumsRule_Plus = DrumsRule_Plus;
		SaveGameInstance->DrumsRule_Minus = DrumsRule_Minus;
		SaveGameInstance->DrumsRule_Branch = DrumsRule_Branch;
		SaveGameInstance->DrumsRule_EndBranch = DrumsRule_EndBranch;

		//Modular system
		SaveGameInstance->ModularAxiom = ModularLSysResetAxiom;
		SaveGameInstance->ModularRule_A = ModularRule_A;
		SaveGameInstance->ModularRule_C = ModularRule_C;
		SaveGameInstance->ModularRule_D = ModularRule_D;
		SaveGameInstance->ModularRule_E = ModularRule_E;
		SaveGameInstance->ModularRule_F = ModularRule_F;
		SaveGameInstance->ModularRule_G = ModularRule_G;
		SaveGameInstance->ModularRule_H = ModularRule_H;
		SaveGameInstance->ModularRule_I = ModularRule_I;
		SaveGameInstance->ModularRule_J = ModularRule_J;
		SaveGameInstance->ModularRule_Plus = ModularRule_Plus;
		SaveGameInstance->ModularRule_Minus = ModularRule_Minus;
		SaveGameInstance->ModularRule_Branch = ModularRule_Branch;
		SaveGameInstance->ModularRule_EndBranch = ModularRule_EndBranch;



		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0))
		{
			// Save succeeded.
			UE_LOG(LogTemp, Warning, TEXT("Save Successful: %s"), *SaveSlotName);
		}
	}
}

void ALSystemSynth::Load_Game(FString SaveSlotName)
{
	ULSystemSaveManager* SaveGameInstance = Cast<ULSystemSaveManager>(UGameplayStatics::CreateSaveGameObject(ULSystemSaveManager::StaticClass()));

	SaveGameInstance = Cast<ULSystemSaveManager>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	LSysResetAxiom = SaveGameInstance->Axiom;
	Rule_A = SaveGameInstance->Rule_A;
	Rule_C = SaveGameInstance->Rule_C;
	Rule_D = SaveGameInstance->Rule_D;
	Rule_E = SaveGameInstance->Rule_E;
	Rule_F = SaveGameInstance->Rule_F;
	Rule_G = SaveGameInstance->Rule_G;
	Rule_H = SaveGameInstance->Rule_H;
	Rule_I = SaveGameInstance->Rule_I;
	Rule_J = SaveGameInstance->Rule_J;
	Rule_Plus = SaveGameInstance->Rule_Plus;
	Rule_Minus = SaveGameInstance->Rule_Minus;
	Rule_Branch = SaveGameInstance->Rule_Branch;
	Rule_EndBranch = SaveGameInstance->Rule_EndBranch;

	NotesLSysResetAxiom = SaveGameInstance->NotesAxiom;
	NotesRule_A = SaveGameInstance->NotesRule_A;
	NotesRule_C = SaveGameInstance->NotesRule_C;
	NotesRule_D = SaveGameInstance->NotesRule_D;
	NotesRule_E = SaveGameInstance->NotesRule_E;
	NotesRule_F = SaveGameInstance->NotesRule_F;
	NotesRule_G = SaveGameInstance->NotesRule_G;
	NotesRule_H = SaveGameInstance->NotesRule_H;
	NotesRule_I = SaveGameInstance->NotesRule_I;
	NotesRule_J = SaveGameInstance->NotesRule_J;
	NotesRule_Plus = SaveGameInstance->NotesRule_Plus;
	NotesRule_Minus = SaveGameInstance->NotesRule_Minus;
	NotesRule_Branch = SaveGameInstance->NotesRule_Branch;
	NotesRule_EndBranch = SaveGameInstance->NotesRule_EndBranch;

	ModularLSysResetAxiom = SaveGameInstance->ModularAxiom;
	ModularRule_A = SaveGameInstance->ModularRule_A;
	ModularRule_C = SaveGameInstance->ModularRule_C;
	ModularRule_D = SaveGameInstance->ModularRule_D;
	ModularRule_E = SaveGameInstance->ModularRule_E;
	ModularRule_F = SaveGameInstance->ModularRule_F;
	ModularRule_G = SaveGameInstance->ModularRule_G;
	ModularRule_H = SaveGameInstance->ModularRule_H;
	ModularRule_I = SaveGameInstance->ModularRule_I;
	ModularRule_J = SaveGameInstance->ModularRule_J;
	ModularRule_Plus = SaveGameInstance->ModularRule_Plus;
	ModularRule_Minus = SaveGameInstance->ModularRule_Minus;
	ModularRule_Branch = SaveGameInstance->ModularRule_Branch;
	ModularRule_EndBranch = SaveGameInstance->ModularRule_EndBranch;

	DrumsLSysResetAxiom = SaveGameInstance->DrumsAxiom;
	DrumsRule_A = SaveGameInstance->DrumsRule_A;
	DrumsRule_C = SaveGameInstance->DrumsRule_C;
	DrumsRule_D = SaveGameInstance->DrumsRule_D;
	DrumsRule_E = SaveGameInstance->DrumsRule_E;
	DrumsRule_F = SaveGameInstance->DrumsRule_F;
	DrumsRule_G = SaveGameInstance->DrumsRule_G;
	DrumsRule_H = SaveGameInstance->DrumsRule_H;
	DrumsRule_I = SaveGameInstance->DrumsRule_I;
	DrumsRule_J = SaveGameInstance->DrumsRule_J;
	DrumsRule_Plus = SaveGameInstance->DrumsRule_Plus;
	DrumsRule_Minus = SaveGameInstance->DrumsRule_Minus;
	DrumsRule_Branch = SaveGameInstance->DrumsRule_Branch;
	DrumsRule_EndBranch = SaveGameInstance->DrumsRule_EndBranch;

	reset = true;
	Drumsreset = true;
	Modularreset = true;
	Notesreset = true;

	UE_LOG(LogTemp, Warning, TEXT("Load Complete: %s"), *SaveSlotName);
}

