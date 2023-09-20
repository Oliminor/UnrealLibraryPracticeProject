// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "MyDebug.h"
#include "Kismet/GameplayStatics.h"
#include "MyWizardCharacter.h"
#include "Math/UnrealMathUtility.h"
#include "PracticeVS/PracticeVSGameModeBase.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
    Health = BaseHealth;
}

ABaseEnemy::~ABaseEnemy()
{
    MyAttackAnimation = nullptr;
    PlayerCharacter = nullptr;
    World = nullptr;
    CustomGameMode = nullptr;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
    PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0); // 0 is the player index
    World = GetWorld();
    CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (isDead) return;
    if (isAttack) return;

    EnemyMovement();
    Avoidance();

}

/// <summary>
/// Rotates and moves forward the enemy actor until it inside the attack radius.
/// </summary>
void ABaseEnemy::EnemyMovement()
{
    if (!PlayerCharacter) return;

    //Direction calculation;
    FVector CurrentLocation = GetActorLocation();
    FVector TargetLocation = FVector::ZeroVector;
    if (PlayerCharacter) TargetLocation = PlayerCharacter->GetActorLocation();
    FVector Direction = TargetLocation - CurrentLocation;
    Direction.Normalize();

    FVector NewLocation = CurrentLocation + Direction * MovementSpeed * GetWorld()->DeltaRealTimeSeconds;

    // Convert the direction into Rotation and offset it with 90 degrees, to face forward the on the right axis.
    FRotator NewRotation = Direction.Rotation() - FRotator(0.f, 90.f, 0.f); 

    NewRotation = FRotator(0, NewRotation.Yaw, NewRotation.Roll);
    NewLocation = FVector(NewLocation.X, NewLocation.Y, 0);


    SetActorLocation(NewLocation);
    SetActorRotation(NewRotation);

    // If the player is close enough it will trigger the Attack animation and damage check
    if (FVector::Distance(CurrentLocation, TargetLocation) < 250 && !isAttack)
    {
        Attack();
    }
}

/// <summary>
/// Avoidance function so the enemies does not stack up and does not need physics calculation.
/// </summary>
void ABaseEnemy::Avoidance()
{
    // Set up the Overlap paramteres
    float avoidanceRadius = 50;
    TArray<FOverlapResult> HitResults;
    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_Pawn); // Specify the object types you want to query
    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(avoidanceRadius);

    // Get the World pointer (you may need to obtain it from your actor or component)
    if (World)
    {
        FVector ActorLocation = GetActorLocation(); // Use the location of your actor

        FVector position = FVector::ZeroVector;
        int nAvoid = 0;

        // Add the pawns to the HitResults TArray
        bool bHit = World->OverlapMultiByObjectType(HitResults, ActorLocation, FQuat::Identity, ObjectParams, CollisionShape);

        if (bHit)
        {
            // HitResults will contain all overlapping actors
            for (const FOverlapResult& HitResult : HitResults)
            {
                AActor* OverlappingActor = HitResult.GetActor();
                if (OverlappingActor)
                {
                    // Only count if the pawn is enemy class
                    if (OverlappingActor != this && OverlappingActor->IsA(ABaseEnemy::StaticClass()))
                    {
                        nAvoid++;
                        position += GetActorLocation() - OverlappingActor->GetActorLocation();
                    }
                }
                OverlappingActor = nullptr;
            }
        }

        // Calculation the direction of the avoid movement
        if (nAvoid > 0)
        {
            position /= nAvoid;
            position.Normalize(); // Normalize the avoidance direction
            position = FMath::Lerp(GetActorLocation(), GetActorLocation() + position, MovementSpeed * GetWorld()->DeltaRealTimeSeconds);
            position = FVector(position.X, position.Y, 0);
            SetActorLocation(position);
        }
    }
}

/// <summary>
/// This interface function called when the pawn is activated by the Objectpool manager
/// </summary>
void ABaseEnemy::PoolObjectInit()
{
    if (CustomGameMode)
    {
        isDead = false;
        MaxHealth = BaseHealth * CustomGameMode->GameManager->GetPlayerLevel();
        Health = MaxHealth;
    }
}

/// <summary>
/// Interface that called when the enemy is damaged
/// </summary>
void ABaseEnemy::IDamageTaken(int DamageValue)
{
    //Calls the hit effect feedback (the material turns into red) when the health is more than 0
    if (Health > 0)  HitEffect(1);

    Health -= DamageValue;
    Health = FMath::Clamp(Health, 0, MaxHealth);

    if (Health <= 0)
    {
        isDead = true;
        CustomGameMode->GameManager->IncreaseTotalEnemeyKilled();

        SetActorEnableCollision(false);

        // Throws the experience crystal by 70% chance
        if (CustomGameMode)
        {
            FRandomStream RandomStream(FMath::Rand());

            float RandomValue = RandomStream.FRand();

            if (RandomValue <= 0.7f)
            {
                AActor* MyActor = CustomGameMode->ObjectPool->GetExperienceCrystal();
                if (MyActor) MyActor->SetActorLocation(GetActorLocation());
                MyActor = nullptr;
            }
        }

        // After 2 seconds the enemy pawn despawns (inactivated by the object pool)
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseEnemy::Death, 2.0f, false);
    }
}

/// <summary>
/// Deactivate the object by the ObjectPool manager
/// </summary>
void ABaseEnemy::Death()
{
    if (CustomGameMode)
    {
        CustomGameMode->ObjectPool->ReturnObject(this);
    }
}

/// <summary>
/// Set the material to red for 0.05 secnds when this pawn damaged
/// </summary>
void ABaseEnemy::HitEffect(float Value)
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

    FTimerHandle WaveTimerHandle;

    if (Value <= 0) return;

    // After 0.05 seconds, the material is changed to the original colour
    GetWorldTimerManager().SetTimer(WaveTimerHandle, [=]() {
        HitEffect(0);
        }, 0.05f, false);
}

/// <summary>
/// Attack function called when the enemy met with the attack conditions
/// </summary>
void ABaseEnemy::Attack()
{
    isAttack = true;
    
    USkeletalMeshComponent* SkeletalMeshComponent = this->FindComponentByClass<USkeletalMeshComponent>();

    // Triggers the attack animation
    if (MyAttackAnimation)
    {
        // Play the animation montage
        UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->PlaySlotAnimationAsDynamicMontage(MyAttackAnimation, FName("DefaultSlot"), 0.0f, 0.0f, 1.5f, 1, 0.0f);
        }
        AnimInstance = nullptr;
    }

    SkeletalMeshComponent = nullptr;

    FTimerHandle AttackTimerHandle;

    // Damage check offset by  0.3 seconds so it matches with the animation (Hard coded, should be changed)
    GetWorldTimerManager().SetTimer(AttackTimerHandle, [=]() {

        AreaDamageCheck(300, Damage);

        }, 0.3f, false);

    FTimerHandle WaveTimerHandle;

    // 1 second cooldown before the next attack (Hard coded, should be changed)
    GetWorldTimerManager().SetTimer(WaveTimerHandle, [=]() {

        isAttack = false;

        }, 1.0f, false);
}

/// <summary>
/// Damage check in a radius
/// </summary>
void ABaseEnemy::AreaDamageCheck(float Radius, int DamageValue)
{
    if (!PlayerCharacter) return;

    // Setup the Overlap parameters
    TArray<FOverlapResult> HitResults;
    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_Pawn); // Specify the object types you want to query
    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(Radius);

    // Get the World pointer (you may need to obtain it from your actor or component)
    if (World)
    {
        FVector ActorLocation = GetActorLocation(); // Use the location of your actor

        bool bHit = World->OverlapMultiByObjectType(HitResults, ActorLocation, FQuat::Identity, ObjectParams, CollisionShape);

        if (bHit)
        {
            // HitResults will contain all overlapping actors
            for (const FOverlapResult& HitResult : HitResults)
            {
                AActor* OverlappingActor = HitResult.GetActor();
                if (OverlappingActor)
                {
                    if (OverlappingActor == PlayerCharacter)
                    {
                        // If the overlapped actor is the player, call the DamageTaken Interface
                        IMyInterface* InterfaceInstance = Cast<IMyInterface>(PlayerCharacter);
                        if (InterfaceInstance)
                        {
                            // Call the interface function
                            InterfaceInstance->IDamageTaken(DamageValue);
                            InterfaceInstance = nullptr;
                            return;
                        }
                    }
                }
                OverlappingActor = nullptr;
            }
        }
    }
}