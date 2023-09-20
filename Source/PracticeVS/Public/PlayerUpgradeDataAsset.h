// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UPlayerPassiveUpgradeDataList;

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerUpgradeDataAsset.generated.h"


UCLASS()
class PRACTICEVS_API UPlayerUpgradeDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillData")
	FString name;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = "SkillData")
	TArray<UPlayerPassiveUpgradeDataList*> PlayerUpgradeDataList;
};
