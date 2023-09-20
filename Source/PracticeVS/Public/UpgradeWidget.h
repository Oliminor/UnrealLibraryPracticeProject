// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UButton;

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEVS_API UUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	FString UpgradeName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interface Variables")
	FString UpgradeDescription = "";

	UButton* MyButton;

	UFUNCTION(BlueprintCallable, Category = "Interface Variables")
	void ButtonPress();

	void SetUpgradeWidget(FString name, FString description, int32 Index);

private:
	FString UpgradeIndex;
};
