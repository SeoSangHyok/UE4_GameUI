// Fill out your copyright notice in the Description page of Project Settings.

#include "practice_umg_GameInstance.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "TableManager.h"


UTableManager* Upractice_umg_GameInstance::TableManager = nullptr;

UTableManager* Upractice_umg_GameInstance::GetTableManager()
{
	if (!IsValid(TableManager))
	{
		TableManager = NewObject<UTableManager>(GetTransientPackage(), UTableManager::StaticClass());
		TableManager->AddToRoot();
	}

	return TableManager;
}

Upractice_umg_GameInstance* Upractice_umg_GameInstance::Get(AActor* Actor)
{
	UWorld* World = GEngine->GetWorldFromContextObject(Actor->GetWorld(), EGetWorldErrorMode::LogAndReturnNull);
	return World ? Cast<Upractice_umg_GameInstance>(World->GetGameInstance()) : nullptr;
}