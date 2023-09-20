// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterface.h"
#include "ExperienceCrystal.generated.h"

UCLASS()
class PRACTICEVS_API AExperienceCrystal : public AActor, public IMyPoolObjectInit
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExperienceCrystal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PoolObjectInit() override;

	void PickedUp();

private:
	void LerpMovement();

private:
	int experience;
	bool IsPickedUp = false;

};
