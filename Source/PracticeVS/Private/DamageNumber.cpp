// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageNumber.h"
#include "MyDebug.h"
#include "Animation/WidgetAnimation.h"

void UDamageNumber::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UDamageNumber::TickActionsAndAnimation(InDeltaTime);
}

/// <summary>
/// Destroy this widget if not needed
/// </summary>
void UDamageNumber::DestroyThis()
{
	RemoveFromParent();
}

/// <summary>
/// After this widget instantiated it plays the animation and destroy itself in 2 seconds
/// </summary>
void UDamageNumber::DamageNumberInit(int32 number, FLinearColor color)
{
	// Plays the animation
	if (FadeAnim) UDamageNumber::PlayAnimation(FadeAnim, 0.0f, 1);

	// Sets the blueprint variables
	DamageNumber = number;
	NumberColor = color;

	// Destroy itself in 2 seconds
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UDamageNumber::DestroyThis, 2.0f, true);
}
