// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardAreaSkill.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "MyDebug.h"
#include "PracticeVS/PracticeVSGameModeBase.h"
#include "Particles/ParticleSystemComponent.h"



void AWizardAreaSkill::Initialize(USkillData* skillData)
{
	Super::Initialize(skillData);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AWizardAreaSkill::TriggerSkill, 0.35f, false);

	UWorld* World = GetWorld();
	if (World)
	{
		UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAttached(
			SkillEffect, // Reference to your particle system
			RootComponent,     // The component to which you want to attach the particle system
			FName(""), // The name of the emitter you want to spawn
			FVector(0), FRotator(0), EAttachLocation::KeepRelativeOffset, true
		);

		float Scale = SkillData->Radius / 150;

		FVector NewScale = FVector(Scale, Scale, Scale); // Adjust the values as needed

		if (ParticleComponent)
		{
			APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

			ParticleComponent->SetRelativeScale3D(NewScale * CustomGameMode->GameManager->PlayerData->Area);

			CustomGameMode = nullptr;
		}
		ParticleComponent = nullptr;
	}

	World = nullptr;
}

void AWizardAreaSkill::TriggerSkill()
{
	APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	AreaDamageCheck(SkillData->Radius * CustomGameMode->GameManager->PlayerData->Area);

	CustomGameMode = nullptr;
}

