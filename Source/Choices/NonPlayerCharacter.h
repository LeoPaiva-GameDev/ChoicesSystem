// Code by Leonardo Paiva

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Subtitle.h"
#include "Components/BoxComponent.h"
#include "NonPlayerCharacter.generated.h"

class AChoicesCharacter;
UCLASS()
class CHOICES_API ANonPlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	
private:

	AChoicesCharacter* Character;
	
	UFUNCTION()
	void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void Talk(USoundBase* Dialog, TArray<FSubtitle> Subtitles);


public:
	// Sets default values for this character's properties
	ANonPlayerCharacter();
	
	void AnswerToPlayer(FName PlayerLine, TArray<FSubtitle>& SubtitlesToDisplay, float Delay);
	
	FORCEINLINE UDataTable* GetPlayerLines() { return PlayerSpeechChoice; } 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
    UBoxComponent* BoxComponent;
    
    UPROPERTY(VisibleAnywhere)
    UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = ChoiceSystem)
    UDataTable* PlayerSpeechChoice;

	UPROPERTY(EditAnywhere, Category = ChoiceSystem)
    UDataTable* NPCLines;
    	


};
