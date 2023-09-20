// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerData.h"
#include "MyDebug.h"

UPlayerData::UPlayerData()
{
	MaxHealth = 0;
	CriticalStrikeRate = 0.0f;
	CriticalDamage = 0.0f;
	Damage = 0.0f;
	FlatDamage = 0.0f;
	MovementSpeed = 0.0f;
	Area = 0.0f;
	Cooldown = 0.0f;
}

void UPlayerData::UpdateDataValue(PlayerStats DataType, float Value)
{
	switch (DataType)
	{
	case PlayerStats::AREA:
		Area += Value;
		break;
	case PlayerStats::COOLDOWN:
		Cooldown += Value;
		break;
	case PlayerStats::CRITICALDAMAGE:
		CriticalDamage += Value;
		break;
	case PlayerStats::CRITICALSTRIKERATE:
		CriticalStrikeRate += Value;
		break;
	case PlayerStats::DAMAGE:
		Damage += Value;
		break;
	case PlayerStats::MAXHEALTH:
		MaxHealth += (int)Value;
		break;
	case PlayerStats::MOVEMENTSPEED:
		MovementSpeed += Value;
		break;
	case PlayerStats::PICKUPAREA:
		PickUpArea += Value;
		break;
	case PlayerStats::FLATDAMAGE:
		FlatDamage += Value;
		break;
	}
}
