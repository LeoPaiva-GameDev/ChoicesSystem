
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NonPlayerCharacter.h"
#include "ChoicesUI.h"
#include "ChoicesCharacter.generated.h"

UCLASS(config=Game)
class AChoicesCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

private:

	bool bIsTalking;

	bool bIsInInteractionRange;

	void ToggleTalking();

	ANonPlayerCharacter* AssociatedNPC;

	UDataTable* AvailableLines;

	FDialog* RetrieveDialog(UDataTable* TableToSearch, FName RowName);

	
public:
	AChoicesCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/////////////////////////////////////////////////////////
	///
	void GeneratePlayerLines(UDataTable& PlayerLines);
 
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> Questions;
	
	UFUNCTION(BlueprintCallable, Category = DialogSystem)
        void Talk(FString Excerpt, TArray<FSubtitle>& Subtitles);
 
	void SetTalkRangeStatus(bool Status) { bIsInInteractionRange = Status; }
 
	void SetAssociatedNPC(ANonPlayerCharacter* Pawn) { AssociatedNPC = Pawn; }
 
	FORCEINLINE UChoicesUI* GetUI() { return UI; }

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//////////////////////////////////////////////////////////////////////////////////////////
	
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComp;
 
	UFUNCTION(BlueprintImplementableEvent, Category = DialogSystem)
    void ToggleUI();
 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UChoicesUI* UI;

protected:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

