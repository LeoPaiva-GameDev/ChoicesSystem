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

class UMGAsyncTask : public FNonAbandonableTask
{
	/*The subtitles that we're going to display*/
	TArray<FSubtitle> Subs;
 
	/*UI Reference*/
	UChoicesUI* ChoicesUI;
 
public:
 
	//Constructor
	UMGAsyncTask(TArray<FSubtitle>& Subs, UChoicesUI* ChoicesUI)
	{
		this->Subs = Subs;
		this->ChoicesUI = ChoicesUI;
	}
 
	/*Function needed by the UE in order to determine what's the tasks' status*/
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UMGAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
 
	/*This function executes each time this thread is active - UE4 searches for a function named DoWord() and executes it*/
	void DoWork()
	{
		for (int32 i = 0; i < Subs.Num(); i++)
		{
			//Sleep means that we pause this thread for the given time
			FPlatformProcess::Sleep(Subs[i].AssociatedTime);
 
			//Update our subtitles after the thread comes back
			ChoicesUI->SubtitleToDisplay = Subs[i].Subtitle;
		}
 
		//Sleep 1 second to let the user read the text
		FPlatformProcess::Sleep(1.f);
 
		//Clear the subtitle
		ChoicesUI->SubtitleToDisplay = FString("");
	}
};
