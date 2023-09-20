// Copyright Epic Games, Inc. All Rights Reserved.


#include "PracticeVSGameModeBase.h"
#include "MyObjectPool.h"
#include "CooldownManager.h"
#include "GameManager.h"
#include "MainInterface.h"
#include "MyDebug.h"

APracticeVSGameModeBase::APracticeVSGameModeBase()
{
    UWorld* World = GetWorld();

    // Instantiate the cooldown manager
    if (World)
    {
        MyDebug::ScreenLogText("PracticeVSGameModeBase");

        CooldownManager = NewObject<ACooldownManager>();
        CooldownManager = World->SpawnActor<ACooldownManager>(ACooldownManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    }
    
    World = nullptr;
}

void APracticeVSGameModeBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    MyDebug::ScreenLogText("Post PracticeVSGameModeBase");

    // Instantiate the GameManager, ObjectPool and Main Interface

    if (GameManagerBP) GameManager = GetWorld()->SpawnActor<AGameManager>(GameManagerBP);

    if (ObjectPoolBP) ObjectPool = GetWorld()->SpawnActor<AMyObjectPool>(ObjectPoolBP);

    if (MainInterfaceBP)
    {
        MainInterface = CreateWidget<UMainInterface>(GetWorld(), MainInterfaceBP);
        MainInterface->AddToViewport();
        MainInterface->SetGameManager(GameManager);
    }
}
