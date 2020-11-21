// Code by Leonardo Paiva


#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Subtitle.h"
#include "DialogAttributes.generated.h"

USTRUCT(BlueprintType)
struct FDialog : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString QuestionExcerpt;

	UPROPERTY(EditAnywhere)
	USoundBase* SFX;

	UPROPERTY(EditAnywhere)
	TArray<FSubtitle> Subtitles;

	UPROPERTY(EditAnywhere)
	bool bShouldAIAnswer;
};
