// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSystemSynth.generated.h"

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
	void NotesLSystem();
	void ModularLSystem();


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
		void Set_Rule_Plus(FString rulePlus);


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_Rule_Minus(FString ruleMinus);


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Set_New_Axiom(FString axiom);





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
		FString Get_Rule_Plus();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_Rule_Minus();


	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		FString Get_New_Axiom();




	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Save_Game();

	UFUNCTION(BlueprintCallable, Category = "L System Functions")
		void Load_Game();



	//      Rhythm L-system 
	FString LSysCurrent = "ACB";
	int i, generation = 0;
	FString LSysNext;
	FString LSysResetAxiom = "ACB";
	bool reset = false, playDrums = false;
	FString Rule_A = "CABE", Rule_C = "CADB", Rule_D = "+ACB", Rule_E = "-AB", Rule_F = "", Rule_G = "", Rule_H = "", Rule_Plus = "", Rule_Minus = "";
	//      Drums L-system 
	FString DrumsLSysCurrent = "ADB";
	int Drumsi, Drumsgeneration = 0;
	FString DrumsLSysNext;
	FString DrumsLSysResetAxiom = "ADB";
	bool Drumsreset = false;
	FString DrumsRule_A = "DBDCB", DrumsRule_C = "DBDAB", DrumsRule_D = "", DrumsRule_E = "", DrumsRule_F = "", DrumsRule_G = "", DrumsRule_H = "", DrumsRule_Plus = "", DrumsRule_Minus = "";
	//      Notes L-system 
	FString NotesLSysCurrent = "++ADBJB";
	int Notesi, Notesgeneration = 0;
	FString NotesLSysNext;
	FString NotesLSysResetAxiom = "ADBJB";
	bool Notesreset = false;
	FString NotesRule_A = "FDB", NotesRule_C = "FHAB", NotesRule_D = "FEJB", NotesRule_E = "CAF-B", NotesRule_F = "DIJB", NotesRule_G = "FJB", NotesRule_H = "GDFAB", NotesRule_I = "FJB", NotesRule_Plus = "IFJEB", NotesRule_Minus = "DAB", NotesRule_J = "HEB";
	//      Modular L-system 
	FString ModularLSysCurrent = "ACDEB";
	int Modulari, Modulargeneration = 0;
	FString ModularLSysNext;
	FString ModularLSysResetAxiom = "ACDEB";
	bool Modularreset = false;
	FString ModularRule_A = "ACDEB", ModularRule_C = "", ModularRule_D = "", ModularRule_E = "", ModularRule_F = "DIAB", ModularRule_G = "FAB", ModularRule_H = "GDFAB", ModularRule_I = "FAB", ModularRule_Plus = "IFJEB", ModularRule_Minus = "DAB", ModularRule_J = "HEB";
	int attack, decay, release;
	float sustain;
	int waveForm;

	std::vector<int> notesToPlayCMajor = {36, 38, 40, 41, 43, 45, 47};
	int octaveShifter = 0;
	int currentNote = 0;
	char c;
	std::vector<std::vector<int>> MajorChords;
	std::vector<std::vector<int>> MinorChords;
	std::vector<std::vector<int>> AugmentedChords;
	std::vector<std::vector<int>> DiminishedChords;
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
