// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WizardSkillBase.h"
#include "WizardSkillData.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEVS_API UWizardSkillData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillData")
	FString name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillData")
	TSubclassOf<AActor> SkillToSpawn;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = "SkillData")
	TArray<USkillData*> skillData;
};
