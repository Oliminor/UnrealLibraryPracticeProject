// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UPlayerPassiveUpgradeData;

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerPassiveUpgradeDataList.generated.h"

// Custom class for the player upgrade as List and the Upgrade description which is showed for the player

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class PRACTICEVS_API UPlayerPassiveUpgradeDataList : public UObject
{
	GENERATED_BODY()
	
public:


	UPROPERTY(EditAnywhere, Category = "UpgradeData")
	FString UpgradeDescription;

	UPROPERTY(EditAnywhere, Category = "UpgradeData")
	TArray<UPlayerPassiveUpgradeData*> PlayerPassiveUpgradeListData;
};
