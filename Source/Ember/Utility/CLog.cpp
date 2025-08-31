#include "CLog.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(Ember, Display, All)

void CLog::Log(const UObject* Class, const TCHAR* FunctionName, int32 InValue, ELogVerbosity::Type InLogType)
{
	switch (InLogType)
	{
		case ELogVerbosity::Type::Display:
			UE_LOG(Ember, Display, L"[%s/%s] %d", *Class->GetName(), FunctionName, InValue);
			break;
		case ELogVerbosity::Type::Warning:
			UE_LOG(Ember, Warning, L"[%s/%s] %d", *Class->GetName(), FunctionName, InValue);
			break;
		case ELogVerbosity::Type::Error:
			UE_LOG(Ember, Error, L"[%s/%s] %d", *Class->GetName(), FunctionName, InValue);
			break;
	}
}

void CLog::Log(const UObject* Class, const TCHAR*FunctionName, float InValue, ELogVerbosity::Type InLogType)
{
	switch (InLogType)
	{
	case ELogVerbosity::Type::Display:
		UE_LOG(Ember, Display, L"[%s/%s] %f", *Class->GetName(), FunctionName, InValue);
		break;
	case ELogVerbosity::Type::Warning:
		UE_LOG(Ember, Warning, L"[%s/%s] %f", *Class->GetName(), FunctionName, InValue);
		break;
	case ELogVerbosity::Type::Error:
		UE_LOG(Ember, Error, L"[%s/%s] %f", *Class->GetName(), FunctionName, InValue);
		break;
	}
	
}

void CLog::Log(const UObject* Class, const TCHAR*FunctionName, const FString& InValue, ELogVerbosity::Type InLogType)
{
	switch (InLogType)
	{
	case ELogVerbosity::Type::Display:
		UE_LOG(Ember, Display, L"[%s/%s] %s", *Class->GetName(), FunctionName, *InValue);
		break;
	case ELogVerbosity::Type::Warning:
		UE_LOG(Ember, Warning, L"[%s/%s] %s", *Class->GetName(), FunctionName, *InValue);
		break;
	case ELogVerbosity::Type::Error:
		UE_LOG(Ember, Error, L"[%s/%s] %s", *Class->GetName(), FunctionName, *InValue);
		break;
	}
}

void CLog::Log(const UObject* Class, const TCHAR*FunctionName, const FVector& InValue, ELogVerbosity::Type InLogType)
{
	switch (InLogType)
	{
	case ELogVerbosity::Type::Display:
		UE_LOG(Ember, Display, L"[%s/%s] %s", *Class->GetName(), FunctionName, *InValue.ToString());
		break;
	case ELogVerbosity::Type::Warning:
		UE_LOG(Ember, Warning, L"[%s/%s] %s", *Class->GetName(), FunctionName, *InValue.ToString());
		break;
	case ELogVerbosity::Type::Error:
		UE_LOG(Ember, Error, L"[%s/%s] %s", *Class->GetName(), FunctionName, *InValue.ToString());
		break;
	}
}

void CLog::Log(const UObject* Class, const TCHAR*FunctionName, const FRotator& InValue, ELogVerbosity::Type InLogType)
{
	switch (InLogType)
	{
	case ELogVerbosity::Type::Display:
		UE_LOG(Ember, Display, L"[%s/%s] %s", *Class->GetName(), FunctionName, *InValue.ToString());
		break;
	case ELogVerbosity::Type::Warning:
		UE_LOG(Ember, Warning, L"[%s/%s] %s", *Class->GetName(), FunctionName, *InValue.ToString());
		break;
	case ELogVerbosity::Type::Error:
		UE_LOG(Ember, Error, L"[%s/%s] %s", *Class->GetName(), FunctionName, *InValue.ToString());
		break;
	}
}

void CLog::Log(int32 InValue)
{
	//GLog->Log("Ember", ELogVerbosity::Display, FString::FromInt(InValue));
	UE_LOG(Ember, Display, L"%d", InValue);
}

void CLog::Log(float InValue)
{
	UE_LOG(Ember, Display, L"%f", InValue);
}

void CLog::Log(const FString& InValue)
{
	UE_LOG(Ember, Display, L"%s", *InValue);
}

void CLog::Log(const FVector& InValue)
{
	UE_LOG(Ember, Display, L"%s", *InValue.ToString());
}

void CLog::Log(const FRotator& InValue)
{
	UE_LOG(Ember, Display, L"%s", *InValue.ToString());
}

void CLog::Log(const UObject* InValue)
{
	FString str;

	if (!!InValue)
		str.Append(InValue->GetName());

	str.Append(!!InValue ? " Not Null" : "Null");

	UE_LOG(Ember, Display, L"%s", *str);
}

void CLog::Log(const FString& InFileName, const FString& InFuncName, int32 InLineNumber)
{
	//C:\\Test\\Test.cpp

	int32 index = 0, length = 0;
	InFileName.FindLastChar(L'\\', index);

	length = InFileName.Len() - 1;
	FString fileName = InFileName.Right(length - index);

	UE_LOG(Ember, Display, L"%s, %s, %d", *fileName, *InFuncName, InLineNumber);
}

void CLog::Log(const FString& InValue, ELogVerbosity::Type InLogType)
{
	switch (InLogType)
	{
	case ELogVerbosity::Type::Display:
		UE_LOG(Ember, Display,  L"%s", *InValue);
		break;
	case ELogVerbosity::Type::Warning:
		UE_LOG(Ember, Warning,  L"%s", *InValue);
		break;
	case ELogVerbosity::Type::Error:
		UE_LOG(Ember, Error, L"%s", *InValue);
		break;
	}
}

void CLog::Print(int32 InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, FString::FromInt(InValue));
}

void CLog::Print(float InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, FString::SanitizeFloat(InValue));
}

void CLog::Print(const FString& InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue);
}

void CLog::Print(const FVector& InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue.ToString());
}

void CLog::Print(const FRotator& InValue, int32 InKey, float InDuration, FColor InColor)
{
	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, InValue.ToString());
}

void CLog::Print(const UObject* InValue, int32 InKey, float InDuration, FColor InColor)
{
	FString str;

	if (!!InValue)
		str.Append(InValue->GetName());

	str.Append(!!InValue ? " Not Null" : "Null");

	GEngine->AddOnScreenDebugMessage(InKey, InDuration, InColor, str);
}

void CLog::Print(const FString& InFileName, const FString& InFuncName, int32 InLineNumber)
{
	int32 index = 0, length = 0;
	InFileName.FindLastChar(L'\\', index);

	length = InFileName.Len() - 1;
	FString fileName = InFileName.Right(length - index);

	FString str = FString::Printf(L"%s, %s, %d", *fileName, *InFuncName, InLineNumber);
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, str);
}