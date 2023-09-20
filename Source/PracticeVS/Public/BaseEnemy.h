// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyInterface.h"
#include "BaseEnemy.generated.h"

class APracticeVSGameModeBase;

UCLASS()
class PRACTICEVS_API ABaseEnemy : public APawn, public IMyInterface, public IMyPoolObjectInit
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseEnemy();
	~ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EnemyMovement();
	virtual void Death();
	virtual void HitEffect(float Value);
	virtual void Attack();
	virtual void AreaDamageCheck(float Radius, int DamageValue);

	void Avoidance();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void IDamageTaken(int DamageValue) override; // Interface Function
	virtual void PoolObjectInit() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimSequence* MyAttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	int BaseHealth;
	int MaxHealth;
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats")
	int Damage;

	UPROPERTY(BlueprintReadWrite, Category = "EnemyStats")
	bool isDead = false;

	UPROPERTY(BlueprintReadWrite, Category = "EnemyStats")
	bool isAttack = false;

	ACharacter* PlayerCharacter;
	UWorld* World;
	APracticeVSGameModeBase* CustomGameMode;
};
