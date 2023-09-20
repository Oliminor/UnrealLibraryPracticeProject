// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class APracticeVSGameModeBase;
class UWizardSkillData;
class UPlayerData;

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyInterface.h"
#include "MyWizardCharacter.generated.h"

UCLASS()
class PRACTICEVS_API AMyWizardCharacter : public ACharacter, public IMyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyWizardCharacter();

private:
	void Movement();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void ExperienceCrystalPickUp();

	void TriggerAttack(UWizardSkillData* WizSkillData);
	void HitEffect(float Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void IDamageTaken(int DamageValue) override;

	void AddSkill(UWizardSkillData* NewSkill);

private:
	float verticalAxis;
	float horizontalAxis;

	APracticeVSGameModeBase* CustomGameMode;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wizard Variables")
	float MovementTransition = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wizard Variables")
	TArray<UWizardSkillData*> SkillData;

	UPROPERTY(Instanced, EditAnywhere, BlueprintReadOnly, Category = "Wizard Variables")
	UPlayerData* CharacterData;
};
