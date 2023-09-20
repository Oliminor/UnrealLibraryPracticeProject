// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeWidget.h"
#include "MyDebug.h"
#include "PracticeVS/PracticeVSGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MyButton = Cast<UButton>(GetWidgetFromName(TEXT("UpgradeButton")));

	MyButton->OnClicked.AddDynamic(this, &UUpgradeWidget::ButtonPress);
}

/// <summary>
/// Calls this function when the player pick new ability after level up
/// </summary>
void UUpgradeWidget::ButtonPress()
{
	UpgradeName = UpgradeName.Replace(*UpgradeIndex, TEXT(""), ESearchCase::CaseSensitive);

	APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (CustomGameMode)
	{
		CustomGameMode->GameManager->IncreaseSkillLevel(UpgradeName);
		CustomGameMode->MainInterface->DeleteUpgradeWidgets();
	}

	CustomGameMode = nullptr;
}

/// <summary>
/// Sets this widget variables to show the skill/upgrade name and description to the palyer
/// </summary>
void UUpgradeWidget::SetUpgradeWidget(FString name, FString description, int32 index)
{
	FString IndexString = FString::Printf(TEXT("%d"), index + 1);
	UpgradeIndex = "[" + IndexString + "]";

	UpgradeName = name + UpgradeIndex;
	UpgradeDescription = description;
}
