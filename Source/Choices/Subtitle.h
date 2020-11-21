// Code by Leonardo Paiva

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Subtitle.generated.h"

USTRUCT(BlueprintType)
struct FSubtitle : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
	FString Subtitle;

	UPROPERTY(EditAnywhere)
	float AssociatedTime;
};
