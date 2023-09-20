// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PRACTICEVS_API MyDebug
{
public:
	MyDebug();
	~MyDebug();

	//String screen message
	void static ScreenLogText(FString message = "", FColor color = FColor::Yellow, float scale = 1.0f);

	//bool screen message
	void static ScreenLog(FString message = "", bool variable = false, FColor color = FColor::Yellow, float scale = 1.0f);
	void static ScreenLog(bool variable = false, FColor color = FColor::Yellow, float scale = 1.0f);

	//Int32 screen message
	void static ScreenLog(FString message = "", int32 variable = 0, FColor color = FColor::Yellow, float scale = 1.0f);
	void static ScreenLog(int32 variable = 0, FColor color = FColor::Yellow, float scale = 1.0f);

	//float screen message
	void static ScreenLog(FString message = "", float variable = 0.f, FColor color = FColor::Yellow, float scale = 1.0f);
	void static ScreenLog(float variable = 0.f, FColor color = FColor::Yellow, float scale = 1.0f);

	//FVector screen message
	void static ScreenLog(FString message = "", FVector variable = FVector(0,0,0), FColor color = FColor::Yellow, float scale = 1.0f);
	void static ScreenLog(FVector variable = FVector(0, 0, 0), FColor color = FColor::Yellow, float scale = 1.0f);

	//FRotator screen message
	void static ScreenLog(FString message = "", FRotator variable = FRotator(0, 0, 0), FColor color = FColor::Yellow, float scale = 1.0f);
	void static ScreenLog(FRotator variable = FRotator(0, 0, 0), FColor color = FColor::Yellow, float scale = 1.0f);

	//String Log
	void static LogText(FString message = "", FColor color = FColor::Yellow, float scale = 1.0f);
	
	//bool Log
	void static Log(FString message = "", bool variable = false, FColor color = FColor::Yellow, float scale = 1.0f);
	void static Log(bool variable = false, FColor color = FColor::Yellow, float scale = 1.0f);
	
	//Int32 Log
	void static Log(FString message = "", int32 variable = 0, FColor color = FColor::Yellow, float scale = 1.0f);
	void static Log(int32 variable = 0, FColor color = FColor::Yellow, float scale = 1.0f);

	//float Log
	void static Log(FString message = "", float variable = 0.f, FColor color = FColor::Yellow, float scale = 1.0f);
	void static Log(float variable = 0.f, FColor color = FColor::Yellow, float scale = 1.0f);

	//FVector Log
	void static Log(FString message = "", FVector variable = FVector(0, 0, 0), FColor color = FColor::Yellow, float scale = 1.0f);
	void static Log(FVector variable = FVector(0, 0, 0), FColor color = FColor::Yellow, float scale = 1.0f);

	//FRotator Log
	void static Log(FString message = "", FRotator variable = FRotator(0, 0, 0), FColor color = FColor::Yellow, float scale = 1.0f);
	void static Log(FRotator variable = FRotator(0, 0, 0), FColor color = FColor::Yellow, float scale = 1.0f);
	
};
