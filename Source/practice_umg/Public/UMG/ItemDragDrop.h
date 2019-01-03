// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDrop.generated.h"

/**
 * 
 */
class UInventoryItem;
class UInventorySlot_WS;

UCLASS()
class PRACTICE_UMG_API UItemDragDrop : public UDragDropOperation
{
	GENERATED_BODY()
	
	
	
public:
	UPROPERTY()
	UInventoryItem* InventoryItemRef;

	UPROPERTY()
	UInventorySlot_WS* FromInventorySlotRef;

	UPROPERTY()
	bool Split = false;
};
