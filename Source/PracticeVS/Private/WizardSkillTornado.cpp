// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardSkillTornado.h"
#include "PracticeVS/PracticeVSGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyWizardCharacter.h"

void AWizardSkillTornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SkillMovement();
}

void AWizardSkillTornado::Initialize(USkillData* skillData)
{
	Super::Initialize(skillData);

	// Grabs the player forward vector
	ForwardVector = PlayerCharacter->GetActorForwardVector();

	float Scale = SkillData->Radius / 100;
	FVector NewScale = FVector(Scale, Scale, Scale); // Adjust the values as needed

	SetActorScale3D(NewScale);
}

/// <summary>
/// Moves the projectile using the player forward vector
/// </summary>
void AWizardSkillTornado::SkillMovement()
{
	APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!CustomGameMode) return;

	// Get the actor's current location
	FVector CurrentLocation = GetActorLocation();

	// Calculate the new location based on direction, speed, and delta time
	FVector NewLocation = CurrentLocation + ForwardVector * SkillData->Speed * GetWorld()->DeltaTimeSeconds;

	// Set the actor's new location
	SetActorLocation(NewLocation);

	FVector Target = RaycastCheck();

	if (CustomGameMode->CooldownManager->IsCooldownFinished("TornadoSkill"))
	{
		AreaDamageCheck(SkillData->Radius * CustomGameMode->GameManager->PlayerData->Area);
		CustomGameMode->CooldownManager->AddCooldown("TornadoSkill", 0.15f);
	}

	CustomGameMode = nullptr;
}
