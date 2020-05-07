// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#include "CTPRO870178Pawn.h"


#include "SynthComponents/EpicSynth1Component.h"

//#include "MySynthComponent.h"
#include "Engine/World.h"
#include <math.h> 

#include "CTPRO870178WheelFront.h"
#include "CTPRO870178WheelRear.h"
#include "CTPRO870178Hud.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Materials/Material.h"
#include "GameFramework/Controller.h"

#ifndef HMD_MODULE_INCLUDED
#define HMD_MODULE_INCLUDED 0
#endif

// Needed for VR Headset
#if HMD_MODULE_INCLUDED
#include "IXRTrackingSystem.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#endif // HMD_MODULE_INCLUDED

const FName ACTPRO870178Pawn::LookUpBinding("LookUp");
const FName ACTPRO870178Pawn::LookRightBinding("LookRight");

#define LOCTEXT_NAMESPACE "VehiclePawn"

ACTPRO870178Pawn::ACTPRO870178Pawn()
{

	/*		this is to be replaced with a UE4 modular synth or granular synth
	//set up synth and set initial freq
	// the rest of the update synth code is at the bottom in the Timer functions
	mySynth = CreateDefaultSubobject<UMySynthComponent>(TEXT("synth"));
	mySynth->SetupAttachment(RootComponent);
	mySynth->bAutoActivate = true;
	mySynth->SetFrequency(880);
	

	mySynth = CreateDefaultSubobject<UModularSynthComponent>(TEXT("synth"));
	mySynth->SetupAttachment(RootComponent);
	mySynth->bAutoActivate = true;
	
	
	timeSynth = CreateDefaultSubobject<UTimeSynthComponent>(TEXT("TimeSynth"));
	timeSynth->SetupAttachment(RootComponent);
	timeSynth->SetBPM(145);
	
	timeSynth->bAutoActivate = true;
	*/
	
	

//	timeSynth->AddQuantizationEventDelegate(ETimeSynthEventQuantization::QuarterNote, FOnQuantizationEventBP::BindUFunction(CountdownHasFinished));

//	FOnQuantizationEvent::Add(&CountdownHasFinished());
	
//	myScriptDelegate.BindUFunction(this, &ACTPRO870178Pawn::CountdownHasFinished);

//	timeSynth->AddQuantizationEventDelegate(ETimeSynthEventQuantization::Bar, FOnQuantizationEventBP::BindUFunction(this, &ACTPRO870178Pawn::CountdownHasFinished));

//	timeSynth->AddQuantizationEventDelegate(ETimeSynthEventQuantization::Bar, );

//	QuantisationEvent = CreateDefaultSubobject<FOnQuantizationEvent>(TEXT("TimeSynth"));



	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicle/Sedan/Sedan_SkelMesh.Sedan_SkelMesh"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicle/Sedan/Sedan_AnimBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	
	// Simulation
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(Vehicle4W->WheelSetups.Num() == 4);

	Vehicle4W->WheelSetups[0].WheelClass = UCTPRO870178WheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("Wheel_Front_Left");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UCTPRO870178WheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("Wheel_Front_Right");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = UCTPRO870178WheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("Wheel_Rear_Left");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -12.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UCTPRO870178WheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("Wheel_Rear_Right");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 12.f, 0.f);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	// Create In-Car camera component 
	InternalCameraOrigin = FVector(0.0f, -40.0f, 120.0f);

	InternalCameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("InternalCameraBase"));
	InternalCameraBase->SetRelativeLocation(InternalCameraOrigin);
	InternalCameraBase->SetupAttachment(GetMesh());

	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->bUsePawnControlRotation = false;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetupAttachment(InternalCameraBase);

	//Setup TextRenderMaterial
	static ConstructorHelpers::FObjectFinder<UMaterial> TextMaterial(TEXT("Material'/Engine/EngineMaterials/AntiAliasedTextMaterialTranslucent.AntiAliasedTextMaterialTranslucent'"));
	
	UMaterialInterface* Material = TextMaterial.Object;

	// Create text render component for in car speed display
	InCarSpeed = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarSpeed"));
	InCarSpeed->SetTextMaterial(Material);
	InCarSpeed->SetRelativeLocation(FVector(70.0f, -75.0f, 99.0f));
	InCarSpeed->SetRelativeRotation(FRotator(18.0f, 180.0f, 0.0f));
	InCarSpeed->SetupAttachment(GetMesh());
	InCarSpeed->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));

	// Create text render component for in car gear display
	InCarGear = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
	InCarGear->SetTextMaterial(Material);
	InCarGear->SetRelativeLocation(FVector(66.0f, -9.0f, 95.0f));	
	InCarGear->SetRelativeRotation(FRotator(25.0f, 180.0f,0.0f));
	InCarGear->SetRelativeScale3D(FVector(1.0f, 0.4f, 0.4f));
	InCarGear->SetupAttachment(GetMesh());
	
	// Colors for the incar gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	// Colors for the in-car gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	bInReverseGear = false;
}

void ACTPRO870178Pawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACTPRO870178Pawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACTPRO870178Pawn::MoveRight);
	PlayerInputComponent->BindAxis("LookUp");
	PlayerInputComponent->BindAxis("LookRight");

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ACTPRO870178Pawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ACTPRO870178Pawn::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ACTPRO870178Pawn::OnToggleCamera);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACTPRO870178Pawn::OnResetVR); 
}

void ACTPRO870178Pawn::MoveForward(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void ACTPRO870178Pawn::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void ACTPRO870178Pawn::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ACTPRO870178Pawn::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ACTPRO870178Pawn::OnToggleCamera()
{
	EnableIncarView(!bInCarCameraActive);
}

void ACTPRO870178Pawn::EnableIncarView(const bool bState, const bool bForce)
{
	if ((bState != bInCarCameraActive) || ( bForce == true ))
	{
		bInCarCameraActive = bState;
		
		if (bState == true)
		{
			OnResetVR();
			Camera->Deactivate();
			InternalCamera->Activate();
		}
		else
		{
			InternalCamera->Deactivate();
			Camera->Activate();
		}
		
		InCarSpeed->SetVisibility(bInCarCameraActive);
		InCarGear->SetVisibility(bInCarCameraActive);
	}
}


void ACTPRO870178Pawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// Setup the flag to say we are in reverse gear
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;
	
	// Update the strings used in the hud (incar and onscreen)
	UpdateHUDStrings();

	// Set the string in the incar hud
	SetupInCarHUD();

	bool bHMDActive = false;
#if HMD_MODULE_INCLUDED
	if ((GEngine->XRSystem.IsValid() == true) && ((GEngine->XRSystem->IsHeadTrackingAllowed() == true) || (GEngine->IsStereoscopic3D() == true)))
	{
		bHMDActive = true;
	}
#endif // HMD_MODULE_INCLUDED
	if (bHMDActive == false)
	{
		if ( (InputComponent) && (bInCarCameraActive == true ))
		{
			FRotator HeadRotation = InternalCamera->GetRelativeRotation();
			HeadRotation.Pitch += InputComponent->GetAxisValue(LookUpBinding);
			HeadRotation.Yaw += InputComponent->GetAxisValue(LookRightBinding);
			InternalCamera->SetRelativeRotation(HeadRotation);
		}
	}
}

void ACTPRO870178Pawn::BeginPlay()
{
	Super::BeginPlay();

	/*

//	timeSynth->AddQuantizationEventDelegate.AddDynamic(this, &everyBeat);
	myQuantEvent.AddDynamic(this, &ACTPRO870178Pawn::everyBeat);
	myQuantEvent.Broadcast(ETimeSynthEventQuantization::QuarterNote, 4, 0.25);
	
	

	i = 0;
	LSysNext = "";
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ACTPRO870178Pawn::AdvanceTimer, 1.0f, true);

	*/

//	timeSynth->AddQuantizationEventDelegate(ETimeSynthEventQuantization::Bar, FOnQuantizationEventBP::BindUFunction(this, &ACTPRO870178Pawn::CountdownHasFinished));



	bool bEnableInCar = false;
#if HMD_MODULE_INCLUDED
	bEnableInCar = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
#endif // HMD_MODULE_INCLUDED
	EnableIncarView(bEnableInCar,true);
}

void ACTPRO870178Pawn::OnResetVR()
{
#if HMD_MODULE_INCLUDED
	if (GEngine->XRSystem.IsValid())
	{
		GEngine->XRSystem->ResetOrientationAndPosition();
		InternalCamera->SetRelativeLocation(InternalCameraOrigin);
		GetController()->SetControlRotation(FRotator());
	}
#endif // HMD_MODULE_INCLUDED
}

void ACTPRO870178Pawn::UpdateHUDStrings()
{
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));
	
	if (bInReverseGear == true)
	{
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else
	{
		int32 Gear = GetVehicleMovement()->GetCurrentGear();
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}	
}

void ACTPRO870178Pawn::SetupInCarHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ((PlayerController != nullptr) && (InCarSpeed != nullptr) && (InCarGear != nullptr) )
	{
		// Setup the text render component strings
		InCarSpeed->SetText(SpeedDisplayString);
		InCarGear->SetText(GearDisplayString);
		
		if (bInReverseGear == false)
		{
			InCarGear->SetTextRenderColor(GearDisplayColor);
		}
		else
		{
			InCarGear->SetTextRenderColor(GearDisplayReverseColor);
		}
	}
}

#undef LOCTEXT_NAMESPACE

// the timer code was modified from official Unreal documentation at https://docs.unrealengine.com/en-US/Programming/Tutorials/VariablesTimersEvents/index.html
void ACTPRO870178Pawn::AdvanceTimer()
{
	--CountdownTime;

	if (CountdownTime < 1)
	{
		//                  stop running the timer
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		CountdownHasFinished();
	}
}

void ACTPRO870178Pawn::CountdownHasFinished()
{
	/*
	std::string::iterator it;
	it = LSysCurrent.begin();
	while (it != LSysCurrent.end())
	{
		UE_LOG(LogTemp, Warning, TEXT("Current generation: %c"), it);
		++it;
	}
	
	/////////		L-SYSTEM
	
	//this print to screen code was found in gamedev.tv forums at: https://community.gamedev.tv/t/print-debug-messages-to-screen-from-c/4764
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,        // don't over wrire previous message, add a new one
			5.0f,   // Duration of message - limits distance messages scroll onto screen
			FColor::Cyan.WithAlpha(64),   // Color and transparancy!
			FString::Printf(TEXT("        %s"), *LSysCurrent)  // Our usual text message format
			);
	}

	
	

	if (LSysCurrent[i] == 'A')
	{
		LSysNext += "AB";
		++i;
		mySynth->NoteOn(69, FMath::RandRange(100, 127), FMath::RandRange(1, 4));

		CountdownTime = FMath::RandRange(1, 4);
		GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ACTPRO870178Pawn::AdvanceTimer, 1.0f, true); // using timeSynthComponent, can I strongly time this?
	}
	else if (LSysCurrent[i] == 'B')
	{
		LSysNext += "A";
		++i;
		mySynth->NoteOn(35, FMath::RandRange(100, 127), FMath::RandRange(1, 4));

		CountdownTime = FMath::RandRange(1, 4);
		GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ACTPRO870178Pawn::AdvanceTimer, 1.0f, true);
	}


				//at this point we have reached the current generation's maximum, and have fully grown the next gen. 
				//Start the next gen, reset + start process again.
	if (i >= LSysCurrent.Len()) {		
		LSysCurrent = LSysNext;
		i = 0;
		LSysNext = "";
	}

	*/


//	mySynth->NoteOn(FMath::RandRange(30, 72), FMath::RandRange(100, 127), FMath::RandRange(1, 4));


	/*			cool, but I don't think I'll code the synth myself, I'll use ue4's modular synth

	if (mySynth->Tremolo) {
		mySynth->Tremolo = false; // set Tremolo effect
	}
	else {
		mySynth->Tremolo = true; // set Tremolo effect
	}

	//			midi note to frequency, long winded way (split out for debugging)
	int randomFreq = FMath::RandRange(12, 115);
	float difference = randomFreq - 69;
	float divisor = difference / 12.0f;
	float power = pow(2.0f, divisor);
	float frequency = power * 440;

	
	UE_LOG(LogTemp, Warning, TEXT("Int Random: %i"), randomFreq);
	UE_LOG(LogTemp, Warning, TEXT("float difference: %f"), difference);
	UE_LOG(LogTemp, Warning, TEXT("float divisor: %f"), divisor);
	UE_LOG(LogTemp, Warning, TEXT("float power: %f"), power);
	UE_LOG(LogTemp, Warning, TEXT("float frequency: %f"), frequency);
	
	//	double MTOF = (pow(2.0f, ((randomFreq - 69.0f) / 12.0f))) * 440.0f;

	mySynth->SetFrequency(frequency); // find midi to frequency values
	
	*/
	CountdownTime = FMath::RandRange(1, 4);
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ACTPRO870178Pawn::AdvanceTimer, 1.0f, true);
	
}
/*
void ACTPRO870178Pawn::everyBeat(ETimeSynthEventQuantization QuantizationType, int32 NumBars, float Beat)
{


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,        // don't over wrire previous message, add a new one
			5.0f,   // Duration of message - limits distance messages scroll onto screen
			FColor::Cyan.WithAlpha(64),   // Color and transparancy!
			FString::Printf(TEXT("THE BROADCAST EVENT IS FIRING"))  // Our usual text message format
			);
	}

}
*/