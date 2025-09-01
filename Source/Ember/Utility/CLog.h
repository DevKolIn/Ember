// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define LogLine(){ CLog::Log(__FILE__, __FUNCTION__, __LINE__); }
#define PrintLine(){ CLog::Print(__FILE__, __FUNCTION__, __LINE__); }
#define DebugLogD(val) CLog::Log(this, TEXT(__FUNCTION__), val,ELogVerbosity::Type::Display)
#define DebugLogW(val) CLog::Log(this, TEXT(__FUNCTION__), val,ELogVerbosity::Type::Warning)
#define DebugLogE(val) CLog::Log(this, TEXT(__FUNCTION__), val, ELogVerbosity::Type::Error)
#define DebugLogDD(val) CLog::Log(val, ELogVerbosity::Type::Display)
#define DebugLogWW(val) CLog::Log(val, ELogVerbosity::Type::Warning)
#define DebugLogEE(val) CLog::Log(val, ELogVerbosity::Type::Error)

UENUM()
enum class ELogType : uint8
{
	Display, Warning, Error
};

class EMBER_API CLog
{
public:
	static void Log(const UObject* Class, const TCHAR* FunctionName,  int32 InValue, ELogVerbosity::Type InLogType = ELogVerbosity::Type::Display);
	static void Log(const UObject* Class, const TCHAR* FunctionName, float InValue, ELogVerbosity::Type InLogType = ELogVerbosity::Type::Display);
	static void Log(const UObject* Class, const TCHAR* FunctionName, const FString& InValue, ELogVerbosity::Type InLogType = ELogVerbosity::Type::Display);
	static void Log(const UObject* Class, const TCHAR* FunctionName, const FVector& InValue, ELogVerbosity::Type InLogType = ELogVerbosity::Type::Display);
	static void Log(const UObject* Class, const TCHAR* FunctionName, const FRotator& InValue, ELogVerbosity::Type InLogType = ELogVerbosity::Type::Display);

	static void Log(int32 InValue);
	static void Log(float InValue);
	static void Log(const FString& InValue);
	static void Log(const FVector& InValue);
	static void Log(const FRotator& InValue);
	static void Log(const UObject* InValue);
	static void Log(const FString& InFileName, const FString& InFuncName, int32 InLineNumber);
	static void Log(const FString& InValue, ELogVerbosity::Type InLogType = ELogVerbosity::Type::Display);
	
	static void Print(int32 InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(float InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(const FString& InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(const FVector& InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(const FRotator& InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(const UObject* InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(const FString& InFileName, const FString& InFuncName, int32 InLineNumber);
};
