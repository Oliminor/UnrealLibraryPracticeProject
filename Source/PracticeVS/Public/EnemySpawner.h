// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UEnemyWaveListData;

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class PRACTICEVS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void InitializeEnemyPool();
	void SpawnEnemy(int NumberOfWave);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyToSpawn")
	UEnemyWaveListData* enemyWaveData;
};
