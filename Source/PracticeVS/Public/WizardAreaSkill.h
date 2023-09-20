// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WizardSkillBase.h"
#include "WizardAreaSkill.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEVS_API AWizardAreaSkill : public AWizardSkillBase
{
	GENERATED_BODY()

public:
	virtual void Initialize(USkillData* skillData) override;

	virtual void TriggerSkill() override;

};
