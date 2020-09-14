// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChoicesCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AChoicesCharacter

AChoicesCharacter::AChoicesCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	////////////////////
	bIsTalking = false;
	bIsInInteractionRange = false;
	AssociatedNPC = nullptr;
 
	AudioComp = CreateDefaultSubobject<UAudioComponent>(FName("AudioComp"));
	AudioComp->SetupAttachment(GetRootComponent());

	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AChoicesCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AChoicesCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AChoicesCharacter::MoveRight);

	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AChoicesCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AChoicesCharacter::LookUpAtRate);

	//////////
	PlayerInputComponent->BindAction("Talk", IE_Pressed, this, &AChoicesCharacter::ToggleTalking);	

	
}




void AChoicesCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AChoicesCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AChoicesCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AChoicesCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AChoicesCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && (!bIsTalking))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AChoicesCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && !bIsTalking)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AChoicesCharacter::ToggleTalking()
{
	if (bIsInInteractionRange)
	{
		//If we are in talk range handle the talk status and the UI
		bIsTalking = !bIsTalking;
		ToggleUI();
		if (bIsTalking && AssociatedNPC)
		{
			//The associated pawn is polite enough to face us when we talk to him!
			FVector Location = AssociatedNPC->GetActorLocation();
			FVector TargetLocation = GetActorLocation();
 
			AssociatedNPC->SetActorRotation((TargetLocation - Location).Rotation());
		}
 
	}
}

FDialog* AChoicesCharacter::RetrieveDialog(UDataTable* TableToSearch, FName RowName)
{
	if(!TableToSearch) return nullptr;
 
	//The table is valid - retrieve the given row if possible
	FString ContextString;
	return TableToSearch->FindRow<FDialog>(RowName, ContextString);
}

void AChoicesCharacter::GeneratePlayerLines(UDataTable& PlayerLines)
{
	//Get all the row names of the table
	TArray<FName> PlayerOptions = PlayerLines.GetRowNames();
 
	//For each row name try to retrieve the contents of the table
	for (auto It : PlayerOptions)
	{
		//Retrieve the contents of the table
		FDialog* Dialog = RetrieveDialog(&PlayerLines, It);
 
		if (Dialog)
		{
			//We retrieved a valid row - populate the questions array with our excerpts
			Questions.Add(Dialog->QuestionExcerpt);
		}
	}
 
	//Make sure to create a reference of the available line for later use
	AvailableLines = &PlayerLines;
}

void AChoicesCharacter::Talk(FString Excerpt, TArray<FSubtitle>& Subtitles)
{
	//Get all the row names based on our stored lines
	TArray<FName> PlayerOptions = AvailableLines->GetRowNames();
 
	for (auto It : PlayerOptions)
	{
		//Search inside the available lines table to find the pressed Excerpt from the UI
		FDialog* Dialog = RetrieveDialog(AvailableLines, It);
 
		if (Dialog && Dialog->QuestionExcerpt == Excerpt)
		{
			//We've found the pressed excerpt - assign our sfx to the audio comp and play it
			AudioComp->SetSound(Dialog->SFX);
			AudioComp->Play();
 
			//Update the corresponding subtitles
			Subtitles = Dialog->Subtitles;
 
 
			if (UI && AssociatedNPC && Dialog->bShouldAIAnswer)
			{
				//Calculate the total displayed time for our subtitles
				//When the subtitles end - the associated pawn will be able to talk to our character
 
				TArray<FSubtitle> SubtitlesToDisplay;
 
				float TotalSubsTime = 0.f;
				
				for (int32 i = 0; i < Subtitles.Num(); i++)
				{
					TotalSubsTime += Subtitles[i].AssociatedTime;
				}
 
				//Just a hardcoded value in order for the AI not to answer right after our subs.
				//It would be better if we expose that to our editor? Sure!
				TotalSubsTime += 1.f;
 
				//Tell the associated pawn to answer to our character after the specified time!
				AssociatedNPC->AnswerToPlayer(It, SubtitlesToDisplay, TotalSubsTime);
 
			}
			else if (!Dialog->bShouldAIAnswer) ToggleTalking();
			break;
 
		}
	}
}
