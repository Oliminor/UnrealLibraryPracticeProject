// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AGameManager;
class UUpgradeWidget;
class UDamageNumber;
class UTextBlock;
class UButton;

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainInterface.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEVS_API UMainInterface : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void UpdatePlayerHealth();
	void UpdateTotalEnemyDeath();
	void UpdateExperienceBar();
	void ShowPlayerData();
	void UpdateTimer();

public: 
	void SetGameManager(AGameManager* GameManagerRef);
	void ShowUpgradeWidget();
	void DeleteUpgradeWidgets();
	void SpawnDamageNumber(FVector position, int32 number, FLinearColor color);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	float HealthProgress = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	FString HealthInText = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	int TotalEnemyDeath = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	float ExperienceProgress = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	FString LevelText = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	FString PlayerDataTexts = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	FString TimerText = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	TSubclassOf<UUpgradeWidget> UpgradeWidgetBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	TSubclassOf<UDamageNumber> DamageNumberBP;

	UButton* RestartButton;

private:
	AGameManager* GameManager;
	TArray<UUpgradeWidget*> SpawnedWidgets;
	UTextBlock* PlayerStatsText;
};
