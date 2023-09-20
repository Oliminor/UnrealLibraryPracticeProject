// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UEnemyWaveData;

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyWaveListData.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEVS_API UEnemyWaveListData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WaveListData")

	TArray<UEnemyWaveData*> EnemyWaveList;
};
