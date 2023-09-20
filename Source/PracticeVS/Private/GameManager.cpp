// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "MyDebug.h"
#include "Kismet/GameplayStatics.h"
#include "PracticeVS/PracticeVSGameModeBase.h"
#include "MyWizardCharacter.h"
#include "Math/Vector.h"
#include "PlayerData.h"
#include "PlayerUpgradeDataAsset.h"
#include "WizardSkillData.h"


// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerData = NewObject<UPlayerData>();
	PlayerLevel = 1;
	ExperienceRequiredToNextLevel = 5;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	for (UPlayerUpgradeDataAsset* UpgradeData : PlayerUpgradeData)
	{
		RegisterPlayerUpgrade(UpgradeData->name, UpgradeData->PlayerUpgradeDataList.Num());
	}


	for (UWizardSkillData* UpgradeSkillData : PlayerSkillsData)
	{
		RegisterSkill(UpgradeSkillData->name, UpgradeSkillData->skillData.Num());
	}
}


// Setters and Getters
void AGameManager::SetPlayerCurrentHealth(int health) { PlayerCurrentHealth = health; }

int AGameManager::GetPlayerCurrentHealth() { return PlayerCurrentHealth; }

int AGameManager::GetPlayerMaxHealth() { return PlayerData->MaxHealth; }

int AGameManager::GetPTotalEnemyKilled() { return TotalEnemyKilled; }

int AGameManager::GetPlayerExperience() { return PlayerExperience; }

int AGameManager::GetExperienceRequiredToNextLevel() { return ExperienceRequiredToNextLevel; }

int AGameManager::GetPlayerLevel() { return PlayerLevel; }

/// <summary>
/// Increases the enemy killed number every time when called
/// </summary>
void AGameManager::IncreaseTotalEnemeyKilled() { TotalEnemyKilled++; }

/// <summary>
/// Register new skill to the list from t
/// </summary>
void AGameManager::AddPlayerSkillData(UWizardSkillData* SkillData)
{
	PlayerSkillsData.Add(SkillData);
}

/// <summary>
/// Sets the Player data which contains every player related stats
/// </summary>
void AGameManager::SetPlayerData(UPlayerData* data) 
{ 
	PlayerData = data; 
	PlayerCurrentHealth = PlayerData->MaxHealth;
}

/// <summary>
/// Sets the player experience and levels up the character if the condition met
/// </summary>
void AGameManager::SetPlayerExperience(int experience) 
{ 
	PlayerExperience += experience; 
	TotalExperience += experience;

	MyDebug::ScreenLog("Experience Gained: ", experience);

	if (PlayerExperience >= ExperienceRequiredToNextLevel)
	{
		// Calculates the next level experience requirement
		PlayerExperience = PlayerExperience - ExperienceRequiredToNextLevel;
		ExperienceRequiredToNextLevel += 10;
		PlayerLevel++;

		APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		// Spawns the Upgrade widget when the player levels up
		if (CustomGameMode)
		{
			CustomGameMode->MainInterface->ShowUpgradeWidget();
		}
		
		CustomGameMode = nullptr;

		MyDebug::ScreenLog("Current level: ", PlayerLevel);
		MyDebug::ScreenLog("Exp needed to next level : ", ExperienceRequiredToNextLevel);
	}
}

/// <summary>
/// Register the player skills
/// </summary>
void AGameManager::RegisterSkill(FString name, int maxLevel)
{
	if (!UpgradeSkillMap.Contains(name))
	{
		UpgradeSkillMap.Add(name, FVector2D(0, maxLevel));
	}
}

/// <summary>
/// Register the player Upgrades (Should reworked the skills and upgrades so one function would be sufficient)
/// </summary>
void AGameManager::RegisterPlayerUpgrade(FString name, int maxLevel)
{
	if (!UpgradePlayerMap.Contains(name))
	{
		UpgradePlayerMap.Add(name, FVector2D(0, maxLevel));
	}
}

/// <summary>
///  Returns the skills or upgrades max level
/// </summary>
int AGameManager::GetSkillLevel(FString name)
{
	if (UpgradeSkillMap.Contains(name)) return UpgradeSkillMap[name].X;

	if (UpgradePlayerMap.Contains(name)) return UpgradePlayerMap[name].X;

	return 0;
}

/// <summary>
/// Returns the skill and upgrade Description (for the upgrade widget)
/// </summary>
FString AGameManager::GetSkillDescription(FString name)
{
	if (UpgradeSkillMap.Contains(name))
	{
		for (UWizardSkillData* SkillDa : PlayerSkillsData)
		{
			if (SkillDa->name == name) return SkillDa->skillData[GetSkillLevel(name)]->UpgradeData;
		}
	}

	if (UpgradePlayerMap.Contains(name))
	{
		for (UPlayerUpgradeDataAsset* PlayerDa : PlayerUpgradeData)
		{
			if (PlayerDa->name == name) return PlayerDa->PlayerUpgradeDataList[GetSkillLevel(name)]->UpgradeDescription;
		}
	}

	return "No Luck, Sorry";
}

/// <summary>
/// Returns the skill or upgrade Max level
/// </summary>
int AGameManager::GetSkillMaxLevel(FString name)
{
	if (UpgradeSkillMap.Contains(name)) return UpgradeSkillMap[name].Y;

	if (UpgradePlayerMap.Contains(name)) return UpgradePlayerMap[name].Y;

	return 0;
}

/// <summary>
/// Returns true if the upgrade or skill max level (does not offer this upgrade anymore)
/// </summary>
bool AGameManager::IsUpgradeMaxLevel(FString name)
{
	if (GetSkillMaxLevel(name) <= GetSkillLevel(name)) return true;
	return false;
}

/// <summary>
/// Increases the skill or upgrade level
/// </summary>
void AGameManager::IncreaseSkillLevel(FString name)
{
	if (UpgradeSkillMap.Contains(name))
	{
		if (UpgradeSkillMap[name].X <= UpgradeSkillMap[name].Y)
		{
			UpdatePlayerStats(name);
			UpgradeSkillMap[name].X++;
		}
	}
	if (UpgradePlayerMap.Contains(name))
	{
		if (UpgradePlayerMap[name].X <= UpgradePlayerMap[name].Y)
		{
			UpdatePlayerStats(name);
			UpgradePlayerMap[name].X++;
		}
	}
}

/// <summary>
/// Updates the player stats after upgrade happened
/// </summary>
void AGameManager::UpdatePlayerStats(FString name)
{
	if (UpgradePlayerMap.Contains(name))
	{
		for (UPlayerUpgradeDataAsset* UpgradeData : PlayerUpgradeData)
		{
			if (UpgradeData->name == name)
			{
				for (UPlayerPassiveUpgradeData* Data : UpgradeData->PlayerUpgradeDataList[GetSkillLevel(name)]->PlayerPassiveUpgradeListData)
				{
					PlayerData->UpdateDataValue(Data->PlayerStats, Data->Value);
					if (Data->PlayerStats == PlayerStats::MAXHEALTH) PlayerCurrentHealth += Data->Value;
				}
			}
		}
	}

	if (UpgradeSkillMap.Contains(name))
	{
		APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
		AMyWizardCharacter* MyCustomCharacter = Cast<AMyWizardCharacter>(PlayerCharacter);

		for (UWizardSkillData* UpgradeSkill : PlayerSkillsData)
		{
			if (UpgradeSkill->name == name)
			{
				if (CustomGameMode)
				{
					if (MyCustomCharacter)
					{
						if (GetSkillLevel(name) == 0) MyCustomCharacter->AddSkill(UpgradeSkill);
					}
					CustomGameMode->CooldownManager->ModifyCooldown(name, UpgradeSkill->skillData[GetSkillLevel(name)]->AttackRate);
				}
			}
		}

		CustomGameMode = nullptr;
		PlayerCharacter = nullptr;
	}
}

/// <summary>
/// Returns X amount of random upgrades or/and skill if the palyer levels up
/// </summary>
TArray<FString> AGameManager::GetRandomUpgrades(int32 UpgradeNum)
{
	int UpgradeAvaliable = 0; // The amount of upgrade and skill avaliable
	TArray<FString> SelectedUpgrade; // Stores every Upgrade name;
	TArray<FString> FinalList; // the final list which only contains the random upgrades
	
	int32 UpgradeNumber = UpgradeNum;
	int32 SkillAndPlayerUpgradeNum = UpgradePlayerMap.Num() + UpgradeSkillMap.Num();
	if (SkillAndPlayerUpgradeNum < UpgradeNumber) UpgradeNumber = SkillAndPlayerUpgradeNum;

	int32 i = 0;

	// Checks the avaliable Skills
	for (const auto& MapElement : UpgradeSkillMap)
	{
		const FString& Key = MapElement.Key;
		if (!IsUpgradeMaxLevel(Key))
		{
			SelectedUpgrade.Add(Key);
			UpgradeAvaliable++;
		}
		i++;
	}

	// Checks the avaliable Upgrades
	for (const auto& MapElement : UpgradePlayerMap)
	{
		const FString& Key = MapElement.Key;
		if (!IsUpgradeMaxLevel(Key))
		{
			SelectedUpgrade.Add(Key);
			UpgradeAvaliable++;
		}
		i++;
	}

	if (UpgradeNumber > UpgradeAvaliable) UpgradeNumber = UpgradeAvaliable;

	// Random select X amount if upgrades and add to the FinalList;
	for (size_t j = 0; j < UpgradeNumber; j++)
	{
		int32 RandomNumber = FMath::RandRange(0, UpgradeAvaliable - 1);

		while (FinalList.Contains(SelectedUpgrade[RandomNumber]))
		{
			RandomNumber = FMath::RandRange(0, UpgradeAvaliable - 1);
		}
		FinalList.Add(SelectedUpgrade[RandomNumber]);
	}
	
	return FinalList;
}

