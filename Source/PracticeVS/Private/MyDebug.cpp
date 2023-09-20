// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDebug.h"

MyDebug::MyDebug()
{
}

MyDebug::~MyDebug()
{
}

// Created my own Debug functions, because the original kind of annoying to use

//String screen message
void MyDebug::ScreenLogText(FString message, FColor color, float scale)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, message, true, FVector2D(scale, scale));
}

//bool screen message
void MyDebug::ScreenLog(FString message, bool variable, FColor color, float scale)
{
	FString BoolAsString = variable ? TEXT("true") : TEXT("false");
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, message + BoolAsString, true, FVector2D(scale, scale));
}

void MyDebug::ScreenLog(bool variable, FColor color, float scale)
{
	FString BoolAsString = variable ? TEXT("true") : TEXT("false");
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, BoolAsString, true, FVector2D(scale, scale));
}

//int32 screen message
void MyDebug::ScreenLog(FString message, int32 variable, FColor color, float scale)
{
	FString intString = FString::Printf(TEXT("%d"), variable);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, message + intString, true, FVector2D(scale, scale));
}

void MyDebug::ScreenLog(int32 variable, FColor color, float scale)
{
	FString intString = FString::Printf(TEXT("%d"), variable);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, intString, true, FVector2D(scale, scale));
}

//float screen message
void MyDebug::ScreenLog(FString message, float variable, FColor color, float scale)
{
	FString floatString = FString::Printf(TEXT("%0.2f"), variable);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, message + floatString, true, FVector2D(scale, scale));
}

void MyDebug::ScreenLog(float variable, FColor color, float scale)
{
	FString floatString = FString::Printf(TEXT("%0.2f"), variable);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, floatString, true, FVector2D(scale, scale));
}

//FVector screen message
void MyDebug::ScreenLog(FString message, FVector variable, FColor color, float scale)
{
	FString VectorAsString = FString::Printf(TEXT("X:%.2f, Y:%.2f, Z:%.2f"), variable.X, variable.Y, variable.Z);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, message + VectorAsString, true, FVector2D(scale, scale));
}

void MyDebug::ScreenLog(FVector variable, FColor color, float scale)
{
	FString VectorAsString = FString::Printf(TEXT("X:%.2f, Y:%.2f, Z:%.2f"), variable.X, variable.Y, variable.Z);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, VectorAsString, true, FVector2D(scale, scale));
}

//FRotator screen message
void MyDebug::ScreenLog(FString message, FRotator variable, FColor color, float scale)
{
	FString RotatorAsString = FString::Printf(TEXT("X:%.2f, Y:%.2f, Z:%.2f"), variable.Pitch, variable.Yaw, variable.Roll);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, message + RotatorAsString, true, FVector2D(scale, scale));
}

void MyDebug::ScreenLog(FRotator variable, FColor color, float scale)
{
	FString RotatorAsString = FString::Printf(TEXT("X:%.2f, Y:%.2f, Z:%.2f"), variable.Pitch, variable.Yaw, variable.Roll);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, color, RotatorAsString, true, FVector2D(scale, scale));
}



//String Log
void MyDebug::LogText(FString message, FColor color, float scale)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(message));
}

//bool Log
void MyDebug::Log(FString message, bool variable, FColor color, float scale)
{
	FString BoolAsString = variable ? TEXT("true") : TEXT("false");
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(message + BoolAsString));
}

void MyDebug::Log(bool variable, FColor color, float scale)
{
	FString BoolAsString = variable ? TEXT("true") : TEXT("false");
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(BoolAsString));
}

//int32 Log
void MyDebug::Log(FString message, int32 variable, FColor color, float scale)
{
	FString intString = FString::Printf(TEXT("%d"), variable);
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(message + intString));
}

void MyDebug::Log(int32 variable, FColor color, float scale)
{
	FString intString = FString::Printf(TEXT("%d"), variable);
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(intString));
}

//float Log
void MyDebug::Log(FString message, float variable, FColor color, float scale)
{
	FString floatString = FString::Printf(TEXT("%f"), variable);
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(message + floatString));
}

void MyDebug::Log(float variable, FColor color, float scale)
{
	FString floatString = FString::Printf(TEXT("%f"), variable);
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(floatString));
}

//FVector Log
void MyDebug::Log(FString message, FVector variable, FColor color, float scale)
{
	FString VectorAsString = FString::Printf(TEXT("X:%f, Y:%f, Z:%f"), variable.X, variable.Y, variable.Z);
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(message + VectorAsString));
}

void MyDebug::Log(FVector variable, FColor color, float scale)
{
	FString VectorAsString = FString::Printf(TEXT("X:%f, Y:%f, Z:%f"), variable.X, variable.Y, variable.Z);
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(VectorAsString));
}

//FRotator Log
void MyDebug::Log(FString message, FRotator variable, FColor color, float scale)
{
	FString RotatorAsString = FString::Printf(TEXT("X:%f, Y:%f, Z:%f"), variable.Pitch, variable.Yaw, variable.Roll);
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(message + RotatorAsString));
}

void MyDebug::Log(FRotator variable, FColor color, float scale)
{
	FString RotatorAsString = FString::Printf(TEXT("X:%f, Y:%f, Z:%f"), variable.Pitch, variable.Yaw, variable.Roll);
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(RotatorAsString));
}
