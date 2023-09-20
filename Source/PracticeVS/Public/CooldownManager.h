// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "CooldownManager.generated.h"

UCLASS()
class PRACTICEVS_API ACooldownManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACooldownManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void ResetCooldowns();
	void CooldownDecrease();


public:
	void AddCooldown(FString name, float _time);
	bool IsCooldownFinished(FString name);
	float GetCooldowTimeLeft(FString name);
	void ModifyCooldown(FString name, float _newTime);


private:	
	TMap<FString, float> CooldownMap;
};
