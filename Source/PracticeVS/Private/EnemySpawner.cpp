// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "MyDebug.h"
#include "Kismet/GameplayStatics.h"
#include "MyWizardCharacter.h"
#include "PracticeVS/PracticeVSGameModeBase.h"
#include "EnemyWaveListData.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
    Super::BeginPlay();

    InitializeEnemyPool();

    SpawnEnemy(0);

}

/// <summary>
/// Initialize the prereferenced objects and spawns 100 copy of them (Hard coded number, should be changed)
/// </summary>
void AEnemySpawner::InitializeEnemyPool()
{
    APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

    if (!enemyWaveData) return;

    if (CustomGameMode)
    {
        for (UEnemyWaveData* WaveData : enemyWaveData->EnemyWaveList)
        {
            CustomGameMode->ObjectPool->InitializePool(WaveData->EnemyToSpawn, 100);
        }
    }

    CustomGameMode = nullptr;
}

/// <summary>
/// Careful with this function, could hurt
/// Spawns enemy based on the referenced DataAsset
/// </summary>
void AEnemySpawner::SpawnEnemy(int value)
{
    if (!enemyWaveData) return; // If the DataAsset empty, return

    if (value >= enemyWaveData->EnemyWaveList.Num()) return; // If the wavelist 0, it will returns

    FRandomStream RandomStream(FMath::Rand());

    static int NumberOfWave = 0;
    NumberOfWave = value;

    UWorld* World = GetWorld();
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    APracticeVSGameModeBase* CustomGameMode = Cast<APracticeVSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

    if (World && PlayerCharacter && PlayerController && CustomGameMode)
    {
        // Calculates the timer delay time between waves
        float WatiTimeBetweenWaves = enemyWaveData->EnemyWaveList[NumberOfWave]->WaveTime +
            enemyWaveData->EnemyWaveList[NumberOfWave]->SpawnEnemyTime *
            enemyWaveData->EnemyWaveList[NumberOfWave]->SpawnEnemyNumber;

        // Sets the wave number
        int32 CurrentWaveNumber = enemyWaveData->EnemyWaveList[NumberOfWave]->WaveNumber;

        // Strange statit variables, otherwise could not modify inside the timer Lambda
        static int WaveCounter = 0;
        static int SpawnCounter = 0;
        WaveCounter = 0;

        // Wave timer lambda starts
        static FTimerHandle WaveTimerHandle;

        GetWorldTimerManager().SetTimer(WaveTimerHandle, [=]() {
            float Radius = 3000;
            int32 Elements = enemyWaveData->EnemyWaveList[NumberOfWave]->SpawnEnemyNumber;
            float AngleStep = 360.0f / Elements;

            static float Angle = 0;
            Angle = FMath::FRandRange(0.f, AngleStep);

            SpawnCounter = 0;

            FVector CenterPosition = PlayerCharacter->GetActorLocation();

            static FTimerHandle SpawnTimerHandle;

            // Enemy spawn timer lambda starts
            GetWorldTimerManager().SetTimer(SpawnTimerHandle, [=]() {

                // claculates the spawn position
                FVector Pos = FVector(FMath::Sin(FMath::DegreesToRadians(Angle)), FMath::Cos(FMath::DegreesToRadians(Angle)), 0.f) * Radius;
                FRotator SpawnRotation = FRotator::ZeroRotator;

                float SpawnDistance = 6500;
                FVector CameraLocation;
                FRotator CameraRotation;
                PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
                Pos.Normalize();

                FVector SpawnPos = CameraLocation + Pos * SpawnDistance;

                SpawnPos = FVector(SpawnPos.X, SpawnPos.Y, 0);

                // Spawns the enemy from ObjectPool
                if (enemyWaveData->EnemyWaveList[NumberOfWave]->EnemyToSpawn)
                {
                    AActor* MyActor = CustomGameMode->ObjectPool->GetObject(enemyWaveData->EnemyWaveList[NumberOfWave]->EnemyToSpawn);
                    if (MyActor) MyActor->SetActorLocation(SpawnPos);
                    MyActor = nullptr;
                }
                else
                {
                    MyDebug::ScreenLogText("No Actor defined", FColor::Red);
                }

                SpawnCounter++;
                Angle = AngleStep * SpawnCounter + FMath::FRandRange(0.f, AngleStep);

                // If all the enemies spawned, it will clear the timer (exits from the lambda)
                if (SpawnCounter >= Elements)
                {
                    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
                }

                }, enemyWaveData->EnemyWaveList[NumberOfWave]->SpawnEnemyTime, true, 0.f);

            WaveCounter++;
            // If the all the waves finished, it will clear the timer (exits from the lambda)
            if (WaveCounter >= CurrentWaveNumber)
            {
                FTimerHandle NextWaveTimeHandler;

                // Timer before the new wave starts
                GetWorldTimerManager().SetTimer(NextWaveTimeHandler, [=]() {

                    NumberOfWave++;
                    // Calls this function again (until every enemy spawned from the list)
                    SpawnEnemy(NumberOfWave);

                    return;

                    }, enemyWaveData->EnemyWaveList[NumberOfWave]->WaitUntiNextWaveSpawn + WatiTimeBetweenWaves, false);

                GetWorldTimerManager().ClearTimer(WaveTimerHandle);

            }

            }, WatiTimeBetweenWaves, true, 0.f);
    }

    World = nullptr;
    PlayerCharacter = nullptr;
    PlayerController = nullptr;
    CustomGameMode = nullptr;
}


