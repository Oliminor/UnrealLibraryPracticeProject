// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerData.generated.h"

// Enum for the player stats

UENUM(BlueprintType)
enum class PlayerStats {
	MAXHEALTH = 0,
	CRITICALSTRIKERATE = 1,
	CRITICALDAMAGE = 2,
	DAMAGE = 3,
	MOVEMENTSPEED = 4,
	AREA = 5,
	COOLDOWN = 6,
	FLATDAMAGE = 7,
	PICKUPAREA = 8
};

// Custom class for the enemy stats

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class PRACTICEVS_API UPlayerData : public UObject
{
	GENERATED_BODY()

public:
	UPlayerData();

	void UpdateDataValue(PlayerStats DataType, float Value);

	UPROPERTY(EditAnywhere, Category = "PlayerData")
	int MaxHealth;
	UPROPERTY(EditAnywhere, Category = "PlayerData")
	float CriticalStrikeRate;
	UPROPERTY(EditAnywhere, Category = "PlayerData")
	float CriticalDamage;
	UPROPERTY(EditAnywhere, Category = "PlayerData")
	float Damage;
	UPROPERTY(EditAnywhere, Category = "PlayerData")
	float FlatDamage;
	UPROPERTY(EditAnywhere, Category = "PlayerData")
	float MovementSpeed;
	UPROPERTY(EditAnywhere, Category = "PlayerData")
	float Area;
	UPROPERTY(EditAnywhere, Category = "PlayerData")
	float Cooldown;
	UPROPERTY(EditAnywhere, Category = "PlayerData")
	float PickUpArea;

};
