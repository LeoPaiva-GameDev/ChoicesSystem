// Code by Leonardo Paiva

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogAttributes.h"
#include "ChoicesUI.generated.h"


UCLASS()
class CHOICES_API UChoicesUI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FString SubtitleToDisplay;

	UFUNCTION(BlueprintCallable, Category = "Dialog")
	void UpdateSubtitles(TArray<FSubtitle> Subtitles);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FString> Questions;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Dialog")
	void Show();
	
};
