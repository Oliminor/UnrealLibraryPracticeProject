// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardClosestEnemySkill.h"
#include "MyWizardCharacter.h"
#include "PracticeVS/PracticeVSGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyDebug.h"

void AWizardClosestEnemySkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SkillMovement();
}

void AWizardClosestEnemySkill::Initialize(USkillData* skillData)
{
	Super::Initialize(skillData);

	EnemyTarget = GetClosestEnemyDirection() - PlayerCharacter->GetActorLocation();
	EnemyTarget = FVector(EnemyTarget.X, EnemyTarget.Y, 0);

	float Scale = SkillData->Radius / 30;
	FVector NewScale = FVector(Scale, Scale, Scale); // Adjust the values as needed

	SetActorScale3D(NewScale);
}

/// <summary>
/// Moves the projectile towards the target
/// </summary>
void AWizardClosestEnemySkill::SkillMovement()
{
	// Normalize the direction vector
	EnemyTarget.Normalize();
	SetActorRotation(EnemyTarget.Rotation());

	// Get the actor's current location
	FVector CurrentLocation = GetActorLocation();

	// Calculate the new location based on direction, speed, and delta time
	FVector NewLocation = CurrentLocation + EnemyTarget * SkillData->Speed * GetWorld()->DeltaTimeSeconds;

	// Set the actor's new location
	SetActorLocation(NewLocation);

	// Checks if enemy target hit and returns it iss position
	FVector Target = RaycastCheck();

	// Damage check
	if (Target != FVector::ZeroVector)
	{
		APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (CustomGameMode) AreaDamageCheck(SkillData->Radius * CustomGameMode->GameManager->PlayerData->Area);
		CustomGameMode = nullptr;
		DestroyThis();
	}
}

