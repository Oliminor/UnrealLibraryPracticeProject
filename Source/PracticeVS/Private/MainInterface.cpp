// Fill out your copyright notice in the Description page of Project Settings.


#include "MainInterface.h"
#include "MyDebug.h"
#include "Math/Vector.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/CanvasPanelSlot.h"
#include "PracticeVS/PracticeVSGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerData.h"
#include "GameManager.h"
#include "UpgradeWidget.h"
#include "DamageNumber.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


void UMainInterface::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerStatsText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerStatTexts")));
	PlayerStatsText->SetVisibility(ESlateVisibility::Hidden);
}

// These functions should not called in Tick, it is just for debug purpose (dirty but quick solution)
void UMainInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdatePlayerHealth();
	UpdateTotalEnemyDeath();
	UpdateExperienceBar();
	UpdateTimer();
}

/// <summary>
/// Updates the player Health texts and progress for the UI widget
/// </summary>
void UMainInterface::UpdatePlayerHealth()
{
	float percent = 0.5f;
	float CurrentHealth = GameManager->GetPlayerCurrentHealth();
	float MaxHealth = GameManager->GetPlayerMaxHealth();

	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	HealthProgress = CurrentHealth / MaxHealth;

	FString StringCurrentHealth = FString::Printf(TEXT("%0.0f"), CurrentHealth);
	FString StringMaxHealth = FString::Printf(TEXT("%0.0f"), MaxHealth);

	HealthInText = StringCurrentHealth + "/" + StringMaxHealth;
}

/// <summary>
/// Updates the enemy death number for the UI idget
/// </summary>
void UMainInterface::UpdateTotalEnemyDeath()
{
	TotalEnemyDeath = GameManager->GetPTotalEnemyKilled();
}

/// <summary>
/// Updates the Experience progress bar for the UI idget
/// </summary>
void UMainInterface::UpdateExperienceBar()
{
	FString StringCurrentLevel = FString::Printf(TEXT("%d"), GameManager->GetPlayerLevel());
	LevelText = "LV " + StringCurrentLevel;

	float percent = 0.5f;
	float CurrentExperience = GameManager->GetPlayerExperience();
	float ExperienceNeeded = GameManager->GetExperienceRequiredToNextLevel();

	ExperienceProgress = CurrentExperience / ExperienceNeeded;

}

/// <summary>
/// Shows the player stats when the enemy levels up
/// </summary>
void UMainInterface::ShowPlayerData()
{
	APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (CustomGameMode)
	{
		FString StringMaxHealth = FString::Printf(TEXT("%d"), CustomGameMode->GameManager->PlayerData->MaxHealth);
		StringMaxHealth = "Max Health: " + StringMaxHealth + "\n";

		FString StringDamage = FString::Printf(TEXT("%0.0f"), CustomGameMode->GameManager->PlayerData->Damage * 100);
		StringDamage = "Damage: " + StringDamage + "% \n";

		FString StringFlatDamage = FString::Printf(TEXT("%0.0f"), CustomGameMode->GameManager->PlayerData->FlatDamage);
		StringFlatDamage = "FlatDamage Bonus: " + StringFlatDamage + "\n";

		FString StringMovementSPeed = FString::Printf(TEXT("%0.0f"), CustomGameMode->GameManager->PlayerData->MovementSpeed);
		StringMovementSPeed = "Movement Speed: " + StringMovementSPeed + "\n";

		FString StringCriticalStrike = FString::Printf(TEXT("%0.0f"), CustomGameMode->GameManager->PlayerData->CriticalStrikeRate);
		StringCriticalStrike = "Critical Strike Rate: " + StringCriticalStrike + "%\n";

		FString StringCriticalDamage = FString::Printf(TEXT("%0.0f"), CustomGameMode->GameManager->PlayerData->CriticalDamage * 100 - 100);
		StringCriticalDamage = "Critical Damage: " + StringCriticalDamage + "%\n";

		FString StringCooldown = FString::Printf(TEXT("%0.0f"), CustomGameMode->GameManager->PlayerData->Cooldown * 100);
		StringCooldown = "Cooldown: " + StringCooldown + "% \n";

		FString StringArea = FString::Printf(TEXT("%0.0f"), CustomGameMode->GameManager->PlayerData->Area * 100);
		StringArea = "Damage Area: " + StringArea + "% \n";

		FString StringPickUpArea = FString::Printf(TEXT("%0.0f"), CustomGameMode->GameManager->PlayerData->PickUpArea);
		StringPickUpArea = "Pickup Range: " + StringPickUpArea + "\n";

		PlayerDataTexts = StringMaxHealth + StringDamage + StringFlatDamage + StringMovementSPeed + StringCriticalStrike
			+ StringCriticalDamage + StringCooldown + StringArea + StringPickUpArea;
	}

	CustomGameMode = nullptr;
}

/// <summary>
/// Updates clock/timer for the UI idget
/// </summary>
void UMainInterface::UpdateTimer()
{
	int32 TotalSeconds = FMath::RoundToInt(GetWorld()->TimeSeconds);
	int32 Minutes = TotalSeconds / 60;
	int32 Seconds = TotalSeconds % 60;

	// Format the result as a string
	TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}

/// <summary>
/// Spawns the Upgrade widget when the player levels up
/// </summary>
void UMainInterface::ShowUpgradeWidget()
{
	APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UGameViewportClient* ViewportClient = GEngine->GameViewport;

	if (!UpgradeWidgetBP) return;

	if (!ViewportClient) return;

	if (!CustomGameMode) return;

	FVector2D ViewportSize;
	ViewportClient->GetViewportSize(ViewportSize);

	// Access ViewportSize.X and ViewportSize.Y for the width and height, respectively
	float ScreenWidth = ViewportSize.X;
	float ScreenHeight = ViewportSize.Y;

	TArray<FString> Upgrades = CustomGameMode->GameManager->GetRandomUpgrades(3);

	if (Upgrades.Num() == 0) return;

	for (size_t i = 0; i < Upgrades.Num(); i++)
	{
		UUpgradeWidget* UpgradeWidget = CreateWidget<UUpgradeWidget>(GetWorld(), UpgradeWidgetBP);
		UpgradeWidget->AddToViewport();

		// Calculates the position based on the current screen resolution
		UpgradeWidget->SetPositionInViewport(FVector2D(ScreenWidth / 2, (ScreenHeight / 6) + (ScreenHeight / 4) * i));

		int32 Index = i;

		FString Name = Upgrades[i];
		int32 UpgradeIndex = CustomGameMode->GameManager->GetSkillLevel(Name);
		FString Description = CustomGameMode->GameManager->GetSkillDescription(Name);

		// Sets the Widget name and description
		UpgradeWidget->SetUpgradeWidget(Name, Description, UpgradeIndex);

		SpawnedWidgets.Add(UpgradeWidget);

		UpgradeWidget = nullptr;
	}

	PlayerStatsText->SetVisibility(ESlateVisibility::Visible);
	// Shows the player stats for the player
	ShowPlayerData();

	CustomGameMode->SetPause(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	ViewportClient = nullptr;
	CustomGameMode = nullptr;
}

/// <summary>
/// Deletes the upgrade widgets after the player selected one from them
/// </summary>
void UMainInterface::DeleteUpgradeWidgets()
{
	MyDebug::ScreenLogText("ButtonTest");

	APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (CustomGameMode) CustomGameMode->ClearPause();

	PlayerStatsText->SetVisibility(ESlateVisibility::Hidden);

	for (UUpgradeWidget* UpgradeWidget : SpawnedWidgets)
	{
		if (UpgradeWidget)
		{
			// Remove the widget from the viewport
			UpgradeWidget->RemoveFromParent();

			// Destroy the widget and release its memory
			UpgradeWidget->ConditionalBeginDestroy();

			// Set the widget reference to null to avoid using it further
			UpgradeWidget = nullptr;
		}
	}

	SpawnedWidgets.Empty();

	CustomGameMode = nullptr;
}

/// <summary>
/// Spawns the damage number when the player hits the enemy
/// </summary>
void UMainInterface::SpawnDamageNumber(FVector position, int32 number, FLinearColor color)
{
	APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	UGameViewportClient* ViewportClient = GEngine->GameViewport;

	if (!ViewportClient) return;


	// Access ViewportSize.X and ViewportSize.Y for the width and height, respectively
	if (CustomGameMode)
	{
		UDamageNumber* DamageWidget = CreateWidget<UDamageNumber>(GetWorld(), DamageNumberBP);

		// Get the 3D world location of the actor
		FVector ActorWorldLocation = position;

		// Convert the world location to screen space
		FVector2D ScreenPosition;
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->ProjectWorldLocationToScreen(ActorWorldLocation, ScreenPosition);
		}

		// Position the widget on the screen
		DamageWidget->SetPositionInViewport(ScreenPosition);
		DamageWidget->AddToViewport();
		DamageWidget->DamageNumberInit(number, color);

		PlayerController = nullptr;
		DamageWidget = nullptr;
	}

	CustomGameMode = nullptr;
	ViewportClient = nullptr;
}

// Setter for the GameManager
void UMainInterface::SetGameManager(AGameManager* GameManagerRef)
{
	GameManager = GameManagerRef;
}

