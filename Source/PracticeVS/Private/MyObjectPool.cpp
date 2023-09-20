// Fill out your copyright notice in the Description page of Project Settings.


#include "MyObjectPool.h"
#include "MyDebug.h"
#include "MyInterface.h"
#include "ExperienceCrystal.h"

// Sets default values
AMyObjectPool::AMyObjectPool() {}

AMyObjectPool::~AMyObjectPool()
{
	for (AActor* Actor : ObjectPool)
	{
		if (IsValid(Actor))
		{
			// Destroy the actor
			Actor->Destroy();
		}
	}

	// Clear the TArray to remove the destroyed actors from the list
	ObjectPool.Empty();
}

// Called when the game starts or when spawned
void AMyObjectPool::BeginPlay()
{
	Super::BeginPlay();	

	InitalizeObjects(ExperienceCrystalBP, 100);
}

/// <summary>
/// Spawns the actors and deactivate them
/// </summary>
void AMyObjectPool::InitalizeObjects(TSubclassOf<AActor> ObjectClass, int32 PoolSize)
{
	UWorld* World = GetWorld();
	if (World)
	{
		// Create and add objects to the pool
		for (int32 i = 0; i < PoolSize; i++)
		{
			AActor* NewObject = World->SpawnActor<AActor>(ObjectClass, FVector::ZeroVector, FRotator::ZeroRotator);

			if (NewObject)
			{
				ActivateObject(NewObject, false);
				ObjectPool.Add(NewObject);
			}

			NewObject = nullptr;
		}
	}
	World = nullptr;
}

/// <summary>
/// Initializes actors
/// </summary>
void AMyObjectPool::InitializePool(TSubclassOf<AActor> ObjectClass, int32 PoolSize)
{
	for (AActor* Object : ObjectPool)
	{
		if (Object->IsA(ObjectClass))
		{
			Object = nullptr;
			return;
		}
	}

	InitalizeObjects(ObjectClass, PoolSize);
}

/// <summary>
/// Returns an avaliable objects, if not avaliable (all of them unsed) it will creates more and return one of those
/// </summary>
AActor* AMyObjectPool::GetObject(TSubclassOf<AActor> ObjectClass)
{
	for (AActor* Object : ObjectPool)
	{
		if (Object)
		{
			// Check if the object is of the desired type
			if (Object->IsHidden() && Object->IsA(ObjectClass))
			{
				ActivateObject(Object, true);

				IMyPoolObjectInit* InterfaceInstance = Cast<IMyPoolObjectInit>(Object);
				if (InterfaceInstance)
				{
					// Call the interface function
					InterfaceInstance->PoolObjectInit();
				}
				InterfaceInstance = nullptr;

				return Object;
			}
		}
	}
	
	InitalizeObjects(ObjectClass, 50);
	
	return GetObject(ObjectClass);

}

/// <summary>
/// Returns Experience Crystal (little bit lazey, could used the original GetObject function)
/// </summary>
AActor* AMyObjectPool::GetExperienceCrystal()
{
	return GetObject(ExperienceCrystalBP);
}

/// <summary>
/// Deactivate the characters after not needed
/// </summary>
void AMyObjectPool::ReturnObject(AActor* ObjectToReturn)
{
	if (ObjectToReturn)
	{
		ActivateObject(ObjectToReturn, false);
	}
}

/// <summary>
/// Activates and Deactivate the actors
/// </summary>
void AMyObjectPool::ActivateObject(AActor* Object, bool Bool)
{
	// Hides visible components
	Object->SetActorHiddenInGame(!Bool);

	// Disables collision components
	Object->SetActorEnableCollision(Bool);

	// Stops the Actor from ticking
	Object->SetActorTickEnabled(Bool);
}


