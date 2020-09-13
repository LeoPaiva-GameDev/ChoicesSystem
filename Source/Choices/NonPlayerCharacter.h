// Code by Leonardo Paiva

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Subtitle.h"
#include "Components/BoxComponent.h"
#include "NonPlayerCharacter.generated.h"

UCLASS()
class CHOICES_API ANonPlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	
private: 
UFUNCTION()
void OnBoxOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndex, bool bFromSweep, const FHitResult& SweepResult);

UFUNCTION()
void OnBoxEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndex);

UFUNCTION()
void Talk(USoundBase* Dialog, TArray<FSubtitle> Subtitles);


public:
	// Sets default values for this character's properties
	ANonPlayerCharacter();
	
	void AnswerToPlayer(FName PlayerLine, TArray<FSubtitle>& SubtitlesToDisplay, float delay);
	
	UDataTable* GetPlayerLines() { return PlayerSpeechChoice; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
    UBoxComponent* BoxComponent;
    
    UPROPERTY(VisibleAnywhere)
    UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = DialogSystem)
    UDataTable* PlayerSpeechChoice;

	UPROPERTY(EditAnywhere, Category = DialogSystem)
    UDataTable* NPCLines;
    	


};
