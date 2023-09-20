// Fill out your copyright notice in the Description page of Project Settings.



#include "MyWizardCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyDebug.h"
#include "Math/UnrealMathUtility.h"
#include "BaseEnemy.h"
#include "ExperienceCrystal.h"
#include "PracticeVS/PracticeVSGameModeBase.h"
#include "WizardSkillData.h"
#include "PlayerData.h"

// Sets default values
AMyWizardCharacter::AMyWizardCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AMyWizardCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector::ZeroVector);

	CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (!CustomGameMode) return;

	// Register the player default stats and skills 
	if (CustomGameMode->GameManager)
	{
		CustomGameMode->GameManager->SetPlayerData(CharacterData);
		CustomGameMode->GameManager->RegisterSkill(SkillData[0]->name, SkillData[0]->skillData.Num());
		CustomGameMode->GameManager->IncreaseSkillLevel(SkillData[0]->name);
		CustomGameMode->GameManager->AddPlayerSkillData(SkillData[0]);
	}
}

// Called every frame
void AMyWizardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);;
	if (!CustomGameMode) return;

	Movement();
	for (UWizardSkillData* Skill : SkillData)
	{
		TriggerAttack(Skill);
	}
	ExperienceCrystalPickUp();
}

/// <summary>
/// Triggers the skill if the cooldown is finished
/// </summary>
void AMyWizardCharacter::TriggerAttack(UWizardSkillData* WizSkillData)
{
	UWorld* World = GetWorld();

	if (WizSkillData && CustomGameMode && World)
	{
		int skillLevel = CustomGameMode->GameManager->GetSkillLevel(WizSkillData->name) - 1;

		if (CustomGameMode->CooldownManager->IsCooldownFinished(WizSkillData->name))
		{
			CustomGameMode->CooldownManager->AddCooldown(WizSkillData->name, WizSkillData->skillData[skillLevel]->AttackRate / CustomGameMode->GameManager->PlayerData->Cooldown);

			int static projectileNumber = 0;
			projectileNumber = 0;

			// Timer if multiple projectile called
			FTimerHandle static FirstWaveTimerHandle;
			GetWorldTimerManager().SetTimer(FirstWaveTimerHandle, [=]() {
				AActor* SkillAttack = World->SpawnActor<AActor>(WizSkillData->SkillToSpawn, GetActorLocation(), FRotator::ZeroRotator);

				AWizardSkillBase* WizardSkill = Cast<AWizardSkillBase>(SkillAttack);
				if (WizardSkill)
				{
					// Call the interface function
					WizardSkill->Initialize(WizSkillData->skillData[skillLevel]);
				}

				projectileNumber++;

				if (projectileNumber >= WizSkillData->skillData[skillLevel]->ProjectileNumber)
				{
					GetWorldTimerManager().ClearTimer(FirstWaveTimerHandle);
				}

				WizardSkill = nullptr;
				SkillAttack = nullptr;

				}, 0.15f, true, 0.f);
		}
	}

	World = nullptr;
}

// Called to bind functionality to input
void AMyWizardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyWizardCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyWizardCharacter::MoveRight);
}

/// <summary>
/// Interface function called when the player suffers damage
/// </summary>
void AMyWizardCharacter::IDamageTaken(int Value)
{
	int CurrentHealth = CustomGameMode->GameManager->GetPlayerCurrentHealth();
	CustomGameMode->GameManager->SetPlayerCurrentHealth(CurrentHealth - Value);

	HitEffect(1);
}

/// <summary>
/// New skill added to the player so it will start use that ability too
/// </summary>
void AMyWizardCharacter::AddSkill(UWizardSkillData* NewSkill)
{
	SkillData.Add(NewSkill);
}

/// <summary>
/// Simple WASD movement skill
/// </summary>
void AMyWizardCharacter::Movement()
{
	FVector Direction = FVector(verticalAxis, horizontalAxis, 0);
	Direction.Normalize();

	FVector CurrentLocation = GetActorLocation();

    FVector NewLocation = CurrentLocation + Direction * CustomGameMode->GameManager->PlayerData->MovementSpeed * GetWorld()->DeltaRealTimeSeconds;

	FRotator NewRotation = Direction.Rotation() - FRotator(0.f, 0.f, 0.f);

	SetActorLocation(NewLocation);

	if (Direction != FVector::ZeroVector)
	{
		// Interpolation so the rotation not instant
		FRotator InterpolatedRotation = FMath::Lerp(GetActorRotation(), Direction.Rotation(), GetWorld()->DeltaRealTimeSeconds * 20);

		SetActorRotation(InterpolatedRotation);

		MovementTransition += GetWorld()->DeltaRealTimeSeconds * 4;
	}
	else
	{
		MovementTransition -= GetWorld()->DeltaRealTimeSeconds * 4;
	}

	MovementTransition = FMath::Clamp(MovementTransition, 0, 1.0f);
}


void AMyWizardCharacter::MoveForward(float Value)
{
	// Move the character forward based on the input axis value
	verticalAxis = Value;
}

void AMyWizardCharacter::MoveRight(float Value)
{
	// Move the character right based on the input axis value
	horizontalAxis = Value;
}

/// <summary>
/// Checks if any experience crystal avaliable in the pick up radius 
/// </summary>
void AMyWizardCharacter::ExperienceCrystalPickUp()
{
	TArray<FOverlapResult> HitResults;
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn); // Specify the object types you want to query
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(CustomGameMode->GameManager->PlayerData->PickUpArea);

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

				if (OverlappingActor && OverlappingActor->IsA(AExperienceCrystal::StaticClass()))
				{
					AExperienceCrystal* CrystalInstance = Cast<AExperienceCrystal>(OverlappingActor);

					// Triggers the pick up function to interpolate the crystals towards the player
					if (CrystalInstance) CrystalInstance->PickedUp(); 
					CrystalInstance = nullptr;
				}

				OverlappingActor = nullptr;
			}
		}
	}

	World = nullptr;
}

/// <summary>
/// When the palyers suffers damage it will turn the material into red and back to normal in 0.15 seconds
/// </summary>
void AMyWizardCharacter::HitEffect(float Value)
{
	USkeletalMeshComponent* SkeletalMeshComponent = this->FindComponentByClass<USkeletalMeshComponent>();

	if (SkeletalMeshComponent)
	{
		UMaterialInstanceDynamic* DynamicMaterial = SkeletalMeshComponent->CreateAndSetMaterialInstanceDynamic(0);

		if (DynamicMaterial)
		{
			// Replace "YourParameterName" with the name of your parameter.
			DynamicMaterial->SetScalarParameterValue(TEXT("HitEffectValue"), Value);
		}

		SkeletalMeshComponent->SetMaterial(0, DynamicMaterial);
		DynamicMaterial = nullptr;
	}

	SkeletalMeshComponent = nullptr;

	if (Value <= 0) return;

	FTimerHandle WaveTimerHandle;

	GetWorldTimerManager().SetTimer(WaveTimerHandle, [=]() {
		HitEffect(0);
		}, 0.15f, false);
}