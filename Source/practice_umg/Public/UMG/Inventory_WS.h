// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/PracticeUMGWidget.h"
#include "Inventory_WS.generated.h"

/**
 * 
 */
class UInventorySlot_WS;

UCLASS()
class PRACTICE_UMG_API UInventory_WS : public UPracticeUMGWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	static UClass* GetWidgetClassStatic();
	virtual UClass* GetWidgetClass() override { return GetWidgetClassStatic(); }

	bool Initialize();

	void ToggleInventory();

	UInventorySlot_WS* GetEmptySlot();
	UInventorySlot_WS* GetSlot(int index);
	int GetSlotIndex(UInventorySlot_WS* InventorySlot);
	
private:
	UPROPERTY(meta = (Bindwidget))
	UInventorySlot_WS* InventorySlot_0;

	UPROPERTY(meta = (Bindwidget))
	UInventorySlot_WS* InventorySlot_1;

	UPROPERTY(meta = (Bindwidget))
	UInventorySlot_WS* InventorySlot_2;

	UPROPERTY(meta = (Bindwidget))
	UInventorySlot_WS* InventorySlot_3;

	UPROPERTY(meta = (Bindwidget))
	UInventorySlot_WS* InventorySlot_4;

	UPROPERTY(meta = (Bindwidget))
	UInventorySlot_WS* InventorySlot_5;

	UPROPERTY(meta = (Bindwidget))
	UInventorySlot_WS* InventorySlot_6;

	UPROPERTY(meta = (Bindwidget))
	UInventorySlot_WS* InventorySlot_7;

	UPROPERTY()
	TArray<UInventorySlot_WS*> InventorySlots;

	bool Showing = false;
};
