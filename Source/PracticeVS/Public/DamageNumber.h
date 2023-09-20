// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumber.generated.h"

class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class PRACTICEVS_API UDamageNumber : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void DestroyThis();

	void DamageNumberInit(int32 number, FLinearColor color);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	int32 DamageNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	FLinearColor NumberColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	UWidgetAnimation* FadeAnim;

private:

};
