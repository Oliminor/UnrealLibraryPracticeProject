// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

class AMyObjectPool;
class ACooldownManager;
class AGameManager;
class UMainInterface;

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PracticeVSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEVS_API APracticeVSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	void PostInitializeComponents();


public:
	APracticeVSGameModeBase();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<APawn> playerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<AGameManager> GameManagerBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<AMyObjectPool> ObjectPoolBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	TSubclassOf<UMainInterface> MainInterfaceBP;

public:
	AMyObjectPool* ObjectPool;
	ACooldownManager* CooldownManager;
	AGameManager* GameManager;
	UMainInterface* MainInterface;
};
