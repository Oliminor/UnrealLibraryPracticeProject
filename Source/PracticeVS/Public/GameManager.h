// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UPlayerUpgradeDataAsset;
class UWizardSkillData;
class UPlayerData;

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"


UCLASS()
class PRACTICEVS_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetPlayerCurrentHealth(int health);
	int GetPlayerCurrentHealth();

	int GetPlayerMaxHealth();

	int GetPTotalEnemyKilled();
	void IncreaseTotalEnemeyKilled();

	void SetPlayerExperience(int experience);
	int GetPlayerExperience();

	int GetExperienceRequiredToNextLevel();

	int GetPlayerLevel();

	void SetPlayerData(UPlayerData* data);


	void AddPlayerSkillData(UWizardSkillData* SkillData);
	void RegisterSkill(FString name, int maxLevel);
	void RegisterPlayerUpgrade(FString name, int maxLevel);
	int GetSkillLevel(FString name);
	FString GetSkillDescription(FString name);
	int GetSkillMaxLevel(FString name);
	bool IsUpgradeMaxLevel(FString name);
	void IncreaseSkillLevel(FString name);
	void UpdatePlayerStats(FString name);
	TArray<FString> GetRandomUpgrades(int32 UpgradeNum);

public:
	UPlayerData* PlayerData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameManager Variables")
	TArray<UPlayerUpgradeDataAsset*> PlayerUpgradeData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameManager Variables")
	TArray<UWizardSkillData*> PlayerSkillsData;

private:
	int PlayerLevel;
	int PlayerExperience;
	int ExperienceRequiredToNextLevel;
	int PlayerCurrentHealth;
	int TotalExperience;
	int TotalEnemyKilled;

	TMap<FString, FVector2D> UpgradeSkillMap;
	TMap<FString, FVector2D> UpgradePlayerMap;

};
