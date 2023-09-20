// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillData.generated.h"

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable)
class PRACTICEVS_API USkillData : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	USkillData();

	UPROPERTY(EditAnywhere, Category = "SkillStats")
	int Damage;
	UPROPERTY(EditAnywhere, Category = "SkillStats")
	int ProjectileNumber;
	UPROPERTY(EditAnywhere, Category = "SkillStats")
	float AttackRate;
	UPROPERTY(EditAnywhere, Category = "SkillStats")
	float Speed;
	UPROPERTY(EditAnywhere, Category = "SkillStats")
	float Radius;
	UPROPERTY(EditAnywhere, Category = "SkillStats")
	FString UpgradeData;
};
