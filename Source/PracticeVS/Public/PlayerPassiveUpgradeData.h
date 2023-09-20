// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerData.h"
#include "PlayerPassiveUpgradeData.generated.h"

// Custom class for the Upgrade, so the gameplay designer able to pich the stats (enum) and the value for it

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class PRACTICEVS_API UPlayerPassiveUpgradeData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "UpgradeData")
	PlayerStats PlayerStats;

	UPROPERTY(EditAnywhere, Category = "UpgradeData")
	float Value;
};