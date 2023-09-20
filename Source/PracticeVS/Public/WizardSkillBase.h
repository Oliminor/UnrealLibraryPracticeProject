// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class USkillData;

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WizardSkillBase.generated.h"

UCLASS()
class PRACTICEVS_API AWizardSkillBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWizardSkillBase();
	~AWizardSkillBase();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Initialize(USkillData* skillData);

	virtual void TriggerSkill();

	virtual void SkillMovement();

	virtual void AreaDamageCheck(float Radius);

	virtual FVector RaycastCheck();

	virtual void DestroyThis();

	virtual int DamageCalculation(FVector EnemyPosition);

	virtual FVector GetClosestEnemyDirection();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillStats")
	UParticleSystem* SkillEffect;

protected:
	USkillData* SkillData;
	ACharacter* PlayerCharacter;
};
