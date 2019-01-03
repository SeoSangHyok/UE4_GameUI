// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Item/ItemTableDef.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "practice_umg_GameInstance.generated.h"

/**
 * 
 */
class UTableManager;

UCLASS()
class PRACTICE_UMG_API Upractice_umg_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
	
public:
	static Upractice_umg_GameInstance* Get(class AActor* Actor);
	static UTableManager* GetTableManager();

private:
	static UTableManager* TableManager;
};
