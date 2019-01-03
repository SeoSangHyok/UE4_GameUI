// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryItem.h"
#include "Item/ItemTableDef.h"
#include "practice_umg_GameInstance.h"
#include "TableManager.h"


const FItemTableRow* UInventoryItem::GetItemTableRow()
{
// 	UTableManager* TableManager = Upractice_umg_GameInstance::GetTableManager();
// 	if (!IsValid(TableManager) || 0 == ItemClassID )
// 	{
// 		return nullptr;
// 	}
// 
// 	const FItemTableRow* ItemTableRow = TableManager->GetTable<FItemTableRow>(ItemClassID);


	return ItemTableRow;
}

EItemEquipSlot UInventoryItem::GetItemEquipSlot()
{
// 	UTableManager* TableManager = Upractice_umg_GameInstance::GetTableManager();
// 	if (!IsValid(TableManager) || 0 == ItemClassID)
// 		return EItemEquipSlot::Unknown;
// 
// 	const FItemTableRow* ItemTableRow = TableManager->GetTable<FItemTableRow>(ItemClassID);
// 	if(nullptr == ItemTableRow)
// 		return EItemEquipSlot::Unknown;

	if (nullptr == ItemTableRow)
	{
		return EItemEquipSlot::Unknown;
	}

	return ItemTableRow->ItemEquipSlot;
}

void UInventoryItem::SetItemInfo(int NewItemClassID, int NewStackCount/* = 1*/)
{
	ItemClassID = NewItemClassID;
	StackCount = NewStackCount;

	UTableManager* TableManager = Upractice_umg_GameInstance::GetTableManager();
	if (IsValid(TableManager) && 0 != ItemClassID )
	{
		ItemTableRow = TableManager->GetTable<FItemTableRow>(ItemClassID);
	}

	Empty = false;
	Equipped = false;
}

void UInventoryItem::CleanUp()
{
	ItemClassID = 0;
	StackCount = 0;
	ItemTableRow = nullptr;
	Empty = true;
	Equipped = false;
}