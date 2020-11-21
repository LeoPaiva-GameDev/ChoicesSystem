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
	TArray<FSubtitle> Subs;
 
	UChoicesUI* ChoicesUI;
 
public:
 
	//Constructor
	UMGAsyncTask(TArray<FSubtitle>& Subs, UChoicesUI* ChoicesUI)
	{
		this->Subs = Subs;
		this->ChoicesUI = ChoicesUI;
	}
 
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UMGAsyncTask, STATGROUP_ThreadPoolAsyncTasks);
	}
 
	//This function executes each time this thread is active
	void DoWork()
	{
		for (int32 i = 0; i < Subs.Num(); i++)
		{
			FPlatformProcess::Sleep(Subs[i].AssociatedTime);
 
			ChoicesUI->SubtitleToDisplay = Subs[i].Subtitle;
		}
 
		FPlatformProcess::Sleep(1.f);
 
		ChoicesUI->SubtitleToDisplay = FString("");
	}
};
