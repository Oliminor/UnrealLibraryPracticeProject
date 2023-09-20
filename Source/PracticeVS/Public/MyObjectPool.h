// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyObjectPool.generated.h"

class AExperienceCrystal;

UCLASS()
class PRACTICEVS_API AMyObjectPool : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMyObjectPool();
    ~AMyObjectPool();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame

    // Function to initialize the object pool
    void InitializePool(TSubclassOf<AActor> ObjectClass, int32 PoolSize);

    AActor* GetObject(TSubclassOf<AActor> ObjectClass);

    AActor* GetExperienceCrystal();

    void ReturnObject(AActor* ObjectToReturn);


private:

    // Function to activate or deactivate an object
    void ActivateObject(AActor* Object, bool Activate);

    void InitalizeObjects(TSubclassOf<AActor> ObjectClass, int32 PoolSize);

    // Object pool
    TArray<AActor*> ObjectPool;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ObjectPoolVariables")
    TSubclassOf<AExperienceCrystal> ExperienceCrystalBP;
};