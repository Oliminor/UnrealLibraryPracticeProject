// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardSkillBase.h"
#include "MyDebug.h"
#include "MyInterface.h"
#include "Kismet/GameplayStatics.h"
#include "MyWizardCharacter.h"
#include "PracticeVS/PracticeVSGameModeBase.h"
#include "DrawDebugHelpers.h"
#include "SkillData.h"
#include "BaseEnemy.h"

// Sets default values
AWizardSkillBase::AWizardSkillBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0); // 0 is the player index
}

AWizardSkillBase::~AWizardSkillBase()
{
	SkillData = nullptr;
	PlayerCharacter = nullptr;
	SkillEffect = nullptr;
}

// Called when the game starts or when spawned
void AWizardSkillBase::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AWizardSkillBase::DestroyThis, 5.0f, false);
}

void AWizardSkillBase::Initialize(USkillData* skillData) 
{ 
	SkillData = skillData; 
}

void AWizardSkillBase::TriggerSkill() {}

void AWizardSkillBase::SkillMovement() {}

/// <summary>
/// Checks actors in a certain radious and damages them if possible 
/// </summary>
void AWizardSkillBase::AreaDamageCheck(float Radius)
{
	if (!PlayerCharacter) return;

	TArray<FOverlapResult> HitResults;
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn); // Specify the object types you want to query
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(Radius);

	// Get the World pointer (you may need to obtain it from your actor or component)
	UWorld* World = GetWorld();
	if (World)
	{
		FVector ActorLocation = GetActorLocation(); // Use the location of your actor

		FVector position = FVector::ZeroVector;
		int nAvoid = 0;

		bool bHit = World->OverlapMultiByObjectType(HitResults, ActorLocation, FQuat::Identity, ObjectParams, CollisionShape);

		if (bHit)
		{
			// HitResults will contain all overlapping actors
			for (const FOverlapResult& HitResult : HitResults)
			{
				AActor* OverlappingActor = HitResult.GetActor();
				if (OverlappingActor)
				{
					if (OverlappingActor != PlayerCharacter)
					{
						
						IMyInterface* InterfaceInstance = Cast<IMyInterface>(OverlappingActor);
						// If the hit actor has damage interface
						if (InterfaceInstance)
						{
							int32 damage = DamageCalculation(OverlappingActor->GetActorLocation());
							// Call the interface function
							InterfaceInstance->IDamageTaken(damage);
						}
						InterfaceInstance = nullptr;
					}
					OverlappingActor = nullptr;
				}
			}
		}
	}
	World = nullptr;
}

/// <summary>
/// Raycast checks if the projectile hit anything in front of it
/// </summary>
FVector AWizardSkillBase::RaycastCheck()
{
	FVector StartLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector EndLocation = StartLocation + ForwardVector;  // Adjust the distance as needed

	FHitResult HitResult;

	// Perform the line trace
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerCharacter);

	bool bHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Pawn,  // Adjust the collision channel as needed
		CollisionParams
	);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();

		// Check if the hit actor is your intended target
		if (HitActor)
		{
			// Hit the intended target
			return HitActor->GetActorLocation();
		}

		HitActor = nullptr;
	}

	// Didn't hit the intended target
	return FVector::ZeroVector;
}

void AWizardSkillBase::DestroyThis()
{
	Destroy();
}

/// <summary>
/// Calculates the damage
/// </summary>
int AWizardSkillBase::DamageCalculation(FVector EnemyPosition)
{
	APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (CustomGameMode)
	{
		int Damage = (SkillData->Damage + CustomGameMode->GameManager->PlayerData->FlatDamage) * CustomGameMode->GameManager->PlayerData->Damage;

		int CriticalRoll = FMath::RandRange(0, 100);

		bool isCritical = false;

		if (CriticalRoll < CustomGameMode->GameManager->PlayerData->CriticalStrikeRate)
		{
			isCritical = true;
			Damage *= CustomGameMode->GameManager->PlayerData->CriticalDamage;
		}

		FColor DamageColor = FColor::White;
		if (isCritical) DamageColor = FColor::Red;

		// Spawns the damage number on the enemy position
		CustomGameMode->MainInterface->SpawnDamageNumber(EnemyPosition, Damage, DamageColor);

		CustomGameMode = nullptr;

		return Damage;
	}

	return 0;
}

/// <summary>
/// Returns the closest enemy position, if no enemy, the position random
/// </summary>
FVector AWizardSkillBase::GetClosestEnemyDirection()
{
	if (!PlayerCharacter) return FVector::ZeroVector;

	FVector TargetPos = FVector::ZeroVector;

	// Random position calculation
	TargetPos.X = FMath::FRandRange(-1.0f, 1.0f);
	TargetPos.Y = FMath::FRandRange(-1.0f, 1.0f);

	// Set up overlap paramteres
	TArray<FOverlapResult> HitResults;
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn); // Specify the object types you want to query
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(2000);

	// Get the World pointer (you may need to obtain it from your actor or component)
	UWorld* World = GetWorld();
	if (World)
	{
		FVector ActorLocation = GetActorLocation(); // Use the location of your actor

		FVector position = FVector::ZeroVector;
		int nAvoid = 0;

		bool bHit = World->OverlapMultiByObjectType(HitResults, ActorLocation, FQuat::Identity, ObjectParams, CollisionShape);

		if (bHit)
		{
			float Distance = std::numeric_limits<float>::infinity();

			// HitResults will contain all overlapping actors
			for (const FOverlapResult& HitResult : HitResults)
			{
				AActor* OverlappingActor = HitResult.GetActor();
				if (OverlappingActor)
				{
					// Check if actually enemy class
					if (OverlappingActor->IsA(ABaseEnemy::StaticClass()))
					{
						// Filter down the closest distance
						float EnemyDistance = FVector::Distance(OverlappingActor->GetActorLocation(), PlayerCharacter->GetActorLocation());
						if (EnemyDistance < Distance)
						{
							TargetPos = OverlappingActor->GetActorLocation();
							Distance = EnemyDistance;
						}

					}
				}

				OverlappingActor = nullptr;
			}
		}
	}
	World = nullptr;

	return TargetPos;
}
