// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSystemSynth.generated.h"

enum Scale
{
	Major = 0, 
	Minor = 1, 
	Lydian = 2, Augmented = 2, 
	Mixolydian = 3, Diminished = 3
};

UCLASS()
class CTPRO870178_API ALSystemSynth : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALSystemSynth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "synth")
		class UModularSynthComponent* mySynth;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "samples", meta = (AllowPrivateAccess = "true"))
		class USoundCue* kick_01;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "samples", meta = (AllowPrivateAccess = "true"))
		class USoundCue* snare_01;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "samples", meta = (AllowPrivateAccess = "true"))
		class USoundCue* hihatClosed_01;







	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void TickLSystem();

	void DrumsLSystem();
	void NotesLSystem(bool arpeggio = false);
	void ModularLSystem();

	// RHYTHM L SYSTEM SETTERS
	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_A(FString ruleA);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_C(FString ruleC);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_D(FString ruleD);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_E(FString ruleE);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_F(FString ruleF);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_G(FString ruleG);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_H(FString ruleH);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_I(FString ruleI);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_J(FString ruleJ);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_Plus(FString rulePlus);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_Minus(FString ruleMinus);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_Branch(FString ruleBranch);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_EndBranch(FString ruleEndBranch);






	//DRUMS L SYSTEM SETTERS

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_A(FString Drum_ruleA);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_C(FString Drum_ruleC);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_D(FString Drum_ruleD);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_E(FString Drum_ruleE);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_F(FString Drum_ruleF);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_G(FString Drum_ruleG);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_H(FString Drum_ruleH);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_I(FString Drum_ruleI);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_J(FString Drum_ruleJ);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_Plus(FString Drum_rulePlus);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_Minus(FString Drum_ruleMinus);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_Branch(FString Drum_ruleBranch);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Drum_Rule_EndBranch(FString Drum_ruleEndBranch);








	//NOTE L SYSTEM SETTERS

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_A(FString Note_ruleA);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_C(FString Note_ruleC);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_D(FString Note_ruleD);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_E(FString Note_ruleE);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_F(FString Note_ruleF);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_G(FString Note_ruleG);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_H(FString Note_ruleH);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_I(FString Note_ruleI);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_J(FString Note_ruleJ);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_Plus(FString Note_rulePlus);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_Minus(FString Note_ruleMinus);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_Branch(FString Note_ruleBranch);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Note_Rule_EndBranch(FString Note_ruleEndBranch);









	//MODULAR L SYSTEM SETTERS

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_A(FString Modular_ruleA);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_C(FString Modular_ruleC);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_D(FString Modular_ruleD);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_E(FString Modular_ruleE);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_F(FString Modular_ruleF);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_G(FString Modular_ruleG);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_H(FString Modular_ruleH);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_I(FString Modular_ruleI);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_J(FString Modular_ruleJ);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_Plus(FString Modular_rulePlus);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_Minus(FString Modular_ruleMinus);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_Branch(FString Modular_ruleBranch);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Modular_Rule_EndBranch(FString Modular_ruleEndBranch);





	// set axiom, reset all l-systems

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_New_Axiom(FString axiom);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_New_Drum_Axiom(FString axiom);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_New_Notes_Axiom(FString axiom);

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_New_Modular_Axiom(FString axiom);





	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_A();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_C();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_D();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_E();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_F();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_G();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_H();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_I();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_J();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_Plus();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_Minus();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_Branch();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_EndBranch();




	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_A();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_C();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_D();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_E();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_F();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_G();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_H();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_I();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_J();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_Plus();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_Minus();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_Branch();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_NotesRule_EndBranch();





	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_A();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_C();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_D();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_E();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_F();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_G();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_H();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_I();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_J();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_Plus();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_Minus();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_Branch();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_ModularRule_EndBranch();





	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_A();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_C();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_D();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_E();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_F();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_G();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_H();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_I();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_J();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_Plus();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_Minus();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_Branch();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_DrumsRule_EndBranch();




	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Save_Game(FString SaveSlotName = "Save1");

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Load_Game(FString SaveSlotName = "Save1");



	//      Rhythm L-system 
	FString LSysCurrent = "ACB";
	int i, generation = 0;
	FString LSysNext;
	FString LSysResetAxiom = "ACB";
	bool reset = false, playDrums = false;

	std::vector<int> branch;
	std::vector<std::vector<int>> stem; //set up a pattern for chords to play (and branch to and from)
	int growthSize = 8;
	int branchesOnTheTree = 0;
	int treeSize;
	int beat = 0, bar = 0;

	FString Rule_A = "CABE", Rule_C = "CADB", Rule_D = "+ACB", Rule_E = "-CAB", Rule_F = "", Rule_G = "", Rule_H = "", Rule_Plus = "CAB]", Rule_Minus = "CAB[", Rule_Branch = "", Rule_EndBranch = "", Rule_I = "", Rule_J = "";
	//      Drums L-system 
	FString DrumsLSysCurrent = "ADBDB";
	int Drumsi, Drumsgeneration = 0;
	FString DrumsLSysNext;
	FString DrumsLSysResetAxiom = "ADBDB";
	bool Drumsreset = false;
	FString DrumsRule_A = "CDBDB", DrumsRule_C = "ADBDB", DrumsRule_D = "", DrumsRule_E = "FHDBHDB", DrumsRule_F = "EGDBGDB", DrumsRule_Plus = "", DrumsRule_Minus = "", DrumsRule_G = "", DrumsRule_H = "", DrumsRule_I = "", DrumsRule_J = "", DrumsRule_Branch = "", DrumsRule_EndBranch = "";
	int kickSnareRandomiser;
	//      Notes L-system 
	FString NotesLSysCurrent = "++AB";
	int Notesi, Notesgeneration = 0;
	FString NotesLSysNext;
	FString NotesLSysResetAxiom = "++AB";
	bool Notesreset = false;
	FString NotesRule_A = "AF", NotesRule_C = "FHAB", NotesRule_D = "FEJB", NotesRule_E = "CAF-B", NotesRule_F = "DIJB", NotesRule_G = "FJB", NotesRule_H = "GDFAB", NotesRule_I = "FJB", NotesRule_Plus = "IFJEB", NotesRule_Minus = "DAB", NotesRule_J = "HEB", NotesRule_Branch = "", NotesRule_EndBranch = "";
	//      Modular L-system 
	FString ModularLSysCurrent = "ACDEB";
	int Modulari, Modulargeneration = 0;
	FString ModularLSysNext;
	FString ModularLSysResetAxiom = "ACDEB";
	bool Modularreset = false;
	FString ModularRule_A = "ACDEB", ModularRule_C = "", ModularRule_D = "", ModularRule_E = "", ModularRule_F = "DIAB", ModularRule_G = "FAB", ModularRule_H = "GDFAB", ModularRule_I = "FAB", ModularRule_Plus = "IFJEB", ModularRule_Minus = "DAB", ModularRule_J = "HEB", ModularRule_Branch = "", ModularRule_EndBranch = "";
	int attack, decay, release;
	float sustain;
	int waveForm;

	std::vector<int> Major_Scale = { 0, 2, 4, 5, 7, 9, 11 };
	std::vector<int> Minor_Scale = { 0, 2, 3, 5, 7, 8, 10 };
	std::vector<int> Lydian_Scale = { 0, 2, 4, 6, 7, 9, 11 };
	std::vector<int> Mixolydian_Scale = { 0, 1, 2, 5, 6, 7, 10 };
	std::vector<std::vector<int>> Scales;
	Scale scaleInUse = Major;
	int noteOfScale = 0;
	int octaveShifter = 0;
	int currentKey = 36; // this is C

	//note branching variables
	int triadNote1, triadNote2, triadNote3;
	std::vector<int> triad;
	std::vector<std::vector<int>> noteBranch;
	std::vector<std::vector<std::vector<int>>> noteStem;
	int noteBranchIterator = 0, noteTriadToPlay = 0;

	char c;
	std::vector<std::vector<int>> MajorChords;
	std::vector<std::vector<int>> MinorChords;
	std::vector<std::vector<int>> AugmentedChords;
	std::vector<std::vector<int>> DiminishedChords;
	std::vector<std::vector<std::vector<int>>> Chords;
	Scale chordTypeInUse = Major;

	bool useMajorChords = true, useMinorChords = false, useAugmentedChords = false, useDiminishedChords = false;
	int chordChooser = 0;
	int maxNoChords = 12;
	bool playArpeggio = false;
	int arpeggioCounter = 0;


private:
	UAudioComponent* Kick_01_Component;
	UAudioComponent* Snare_01_Component;
	UAudioComponent* HihatClosed_01_Component;
};
