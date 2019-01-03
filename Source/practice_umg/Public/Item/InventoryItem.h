// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
enum class EItemEquipSlot : uint8;
struct FItemTableRow;


UCLASS()
class PRACTICE_UMG_API UInventoryItem : public UObject
{
	GENERATED_BODY()
	
	
	
public:
	const FItemTableRow* GetItemTableRow();
	EItemEquipSlot GetItemEquipSlot();

	void SetItemInfo(int NewItemClassID, int NewStackCount = 1);
	void CleanUp();

public:
	UPROPERTY()
	int ItemClassID = 0;

	UPROPERTY()
	bool Empty = true;

	UPROPERTY()
	bool Equipped = false;

	UPROPERTY()
	int32 StackCount = 1;

private:
	const FItemTableRow* ItemTableRow = nullptr;
};
