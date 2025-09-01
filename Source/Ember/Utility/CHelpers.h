// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class EMBER_API CHelpers
{
public:
	template<typename T>
	static void CreateComponent(TObjectPtr<AActor> InActor, TObjectPtr<T>* OutComponent, FName InName, TObjectPtr<USceneComponent> InParent = nullptr, FName InSocketName = NAME_None)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
		
		if (InParent != nullptr)
		{
			(*OutComponent)->SetupAttachment(InParent, InSocketName);
			return;
		}

		InActor->SetRootComponent(*OutComponent);
	}

	template<typename T>
	static void CreateActorComponent(TObjectPtr<AActor> InActor, TObjectPtr<T>* OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
	}

	template<typename T>
	static void GetAsset(TObjectPtr<T>* OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		*OutClass = asset.Class;
	}

	template<typename T>
	static TObjectPtr<T> FindActor(TObjectPtr<UWorld> InWorld)
	{
		for (const TObjectPtr<AActor>& actor : InWorld->GetCurrentLevel()->Actors)
			if (actor != nullptr && actor.IsA<T>())
				return Cast<T>(actor);

		return nullptr;
	}

	template<typename T>
	static void FindActors(TObjectPtr<UWorld> InWorld, TArray<TObjectPtr<T>>& OutActors)
	{
		for (const TObjectPtr<AActor>& actor : InWorld->GetCurrentLevel()->Actors)
			if (actor != nullptr && actor.IsA<T>())
				OutActors.Add(Cast<T>(actor));
	}

	template<typename T>
	static TObjectPtr<T> GetComponent(TObjectPtr<AActor> InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	template<typename T>
	static TObjectPtr<T> GetComponent(TObjectPtr<AActor> InActor, const FString& InName)
	{
		TArray<TObjectPtr<T>> components;
		InActor->GetComponents<T>(components);

		for (TObjectPtr<T> component : components)
			if (component.GetName() == InName)
				return component;
		return nullptr;
	}
	template<typename T>
	static TObjectPtr<T> GetComponentCheck(TObjectPtr<AActor> InActor)
	{
		return CastChecked<T>(InActor->GetComponentByClass(T::StaticClass()));
	}
};
