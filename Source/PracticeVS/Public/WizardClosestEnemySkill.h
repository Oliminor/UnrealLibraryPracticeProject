// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WizardSkillBase.h"
#include "WizardClosestEnemySkill.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEVS_API AWizardClosestEnemySkill : public AWizardSkillBase
{
	GENERATED_BODY()
	
public:
	void Tick(float DeltaTime) override;

	void Initialize(USkillData* skillData) override;

private:

	void SkillMovement() override;

private:
	
	FVector EnemyTarget;
};
