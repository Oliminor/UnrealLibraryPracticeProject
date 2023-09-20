// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyWaveData.generated.h"

// EnemyWaveData is stores the individual Wave data


UCLASS()
class PRACTICEVS_API UEnemyWaveData : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UEnemyWaveData();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WaveData")
    TSubclassOf<APawn> EnemyToSpawn; // The enemy pawn to spawn

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WaveData")
    int32 WaveNumber; // the number of wave

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WaveData")
    int32 SpawnEnemyNumber; // the number of enemy

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WaveData")
    float WaveTime; // the time between waves

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WaveData")
    float SpawnEnemyTime; // the time between enemy spawns

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WaveData")
    float WaitUntiNextWaveSpawn; // thetime after the last wave (And before start the new Wave with the new Enemy)
};
