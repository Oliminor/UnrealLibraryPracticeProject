// Fill out your copyright notice in the Description page of Project Settings.


#include "ExperienceCrystal.h"
#include "PracticeVS/PracticeVSGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "MyWizardCharacter.h"
#include "MyDebug.h"

// Sets default values
AExperienceCrystal::AExperienceCrystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	experience = 1;
}

// Called when the game starts or when spawned
void AExperienceCrystal::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AExperienceCrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    LerpMovement();
}

/// <summary>
/// Objectpool manager calls this interface function when the experience crystal spawned
/// </summary>
void AExperienceCrystal::PoolObjectInit()
{
    APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

    if (CustomGameMode)
    {
        experience = 1 * (CustomGameMode->GameManager->GetPlayerLevel() / 2) + 1; // The experience amount increases by the level of the player
        experience = FMath::Clamp(experience, 1, experience);
        IsPickedUp = false;
    }

    CustomGameMode = nullptr;
}

/// <summary>
/// Called when the player is in the pick up range
/// </summary>
void AExperienceCrystal::PickedUp()
{
    IsPickedUp = true;
}

/// <summary>
/// Lerp movement towards the player 
/// </summary>
void AExperienceCrystal::LerpMovement()
{
    if (IsPickedUp)
    {
        ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0); // 0 is the player index
        APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

        if (PlayerCharacter && CustomGameMode)
        {
            FVector CurrentLocation = GetActorLocation();
            FVector TargetLocation = PlayerCharacter->GetActorLocation();
            FVector NewLocation = FMath::Lerp(CurrentLocation, TargetLocation, 10 * GetWorld()->DeltaRealTimeSeconds);

            // Update this actor's location
            SetActorLocation(NewLocation);

            // If the distance close enough it will deactivated by the pool and increases the player experience
            if (FVector::Distance(CurrentLocation, TargetLocation) < 150)
            {
                CustomGameMode->GameManager->SetPlayerExperience(experience);
                CustomGameMode->ObjectPool->ReturnObject(this);
            }
        }

        PlayerCharacter = nullptr;
        CustomGameMode = nullptr;
    }
}

