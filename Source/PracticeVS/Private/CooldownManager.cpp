// Fill out your copyright notice in the Description page of Project Settings.


#include "CooldownManager.h"
#include "MyDebug.h"

// Sets default values
ACooldownManager::ACooldownManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACooldownManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACooldownManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CooldownDecrease();
}

/// <summary>
/// Register the cooldowns if it is not already there
/// </summary>
void ACooldownManager::AddCooldown(FString name, float _time)
{
	if (!CooldownMap.Contains(name))
	{
		CooldownMap.Add(name, _time);
	}
}

/// <summary>
/// Resets all the cooldowns to 0
/// </summary>
void ACooldownManager::ResetCooldowns()
{
	for (auto It = CooldownMap.CreateIterator(); It; ++It)
	{
		FString Key = It.Key();
		float Value = It.Value();

		Value = 0;
	}
}

/// <summary>
/// Decrease the registered cooldowns using deltatime
/// </summary>
void ACooldownManager::CooldownDecrease()
{
	for (auto It = CooldownMap.CreateIterator(); It; ++It)
	{
		It.Value() -= GetWorld()->DeltaRealTimeSeconds;

		// Check if you want to delete the current element based on some condition
		if (It.Value() <= 0.0f)
		{
			It.RemoveCurrent(); // This removes the current key-value pair from the map
		}
	}
}

/// <summary>
/// Check  is the cooldown is finished or not
/// </summary>
bool ACooldownManager::IsCooldownFinished(FString name)
{
	if (!CooldownMap.Contains(name)) return true;

	return false;
}

/// <summary>
/// Returns the amount of time left from the cooldown
/// </summary>
float ACooldownManager::GetCooldowTimeLeft(FString name)
{
	if (!CooldownMap.Contains(name)) return 0;

	return CooldownMap[name];
}

/// <summary>
/// Modify the cooldown time if needed
/// </summary>
void ACooldownManager::ModifyCooldown(FString name, float _newTime)
{
	if (!CooldownMap.Contains(name)) return;

	CooldownMap[name] = _newTime;
}

