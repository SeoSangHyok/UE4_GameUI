// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory_WS.h"
#include "UMG/InventorySlot_WS.h"



UClass* UInventory_WS::GetWidgetClassStatic()
{
	FSoftObjectPath AssetLoader(TEXT("WidgetBlueprint'/Game/UMG/Inventory_WB.Inventory_WB_C'"));
	UClass* Object = Cast<UClass>(AssetLoader.TryLoad());
	if (!IsValid(Object))
		return nullptr;

	return Object;
}

void UInventory_WS::NativeConstruct()
{
	Super::NativeConstruct();

	UWidgetAnimation** InitAnimation = AnimationList.Find(FName(TEXT("InitInventory")));
	if (nullptr != InitAnimation)
	{
		PlayAnimation(*InitAnimation);
	}
}

bool UInventory_WS::Initialize()
{
	Super::Initialize();

// 변수명과 리플렉션 기능을 이용해서 아래처럼 처리도 가능
// 	for (TFieldIterator<UObjectProperty> PropIt(GetClass()); PropIt; ++PropIt)
// 	{
// 		UObjectProperty* Property = *PropIt;
// 		if (IsValid(Property))
// 		{
// 			UObject* TestSlotObject = Property->GetObjectPropertyValue_InContainer(this);
// 			UInventorySlot_WS* WidgetSlot = Cast<UInventorySlot_WS>(TestSlotObject);
// 			if (IsValid(WidgetSlot))
// 			{
// 				InventorySlots.Add(WidgetSlot);
// 			}
// 		}
// 	}
//
// 또는
//	UObjectProperty* ObjProperty = FindField<UObjectProperty>(GetClass(), FName(TEXT("[변수명]")));

	InventorySlots.Add(InventorySlot_0);
	InventorySlot_0->SlotIndex = 0;
	InventorySlots.Add(InventorySlot_1);
	InventorySlot_1->SlotIndex = 1;
	InventorySlots.Add(InventorySlot_2);
	InventorySlot_2->SlotIndex = 2;
	InventorySlots.Add(InventorySlot_3);
	InventorySlot_3->SlotIndex = 3;
	InventorySlots.Add(InventorySlot_4);
	InventorySlot_4->SlotIndex = 4;
	InventorySlots.Add(InventorySlot_5);
	InventorySlot_5->SlotIndex = 5;
	InventorySlots.Add(InventorySlot_6);
	InventorySlot_6->SlotIndex = 6;
	InventorySlots.Add(InventorySlot_7);
	InventorySlot_7->SlotIndex = 7;

	return true;
}

void UInventory_WS::ToggleInventory()
{
	Showing = !Showing;

	UWidgetAnimation** ShowAnimation = AnimationList.Find(FName(TEXT("Showing")));
	if (nullptr == ShowAnimation)
		return;

	if (Showing)
	{
		if (IsPlayingAnimation())
		{
			ReverseAnimation(*ShowAnimation);
		}
		else
		{
			PlayAnimation(*ShowAnimation);
		}
	}
	else
	{
		if (IsPlayingAnimation())
		{
			ReverseAnimation(*ShowAnimation);
		}
		else
		{
			PlayAnimation(*ShowAnimation,0.0f,1,EUMGSequencePlayMode::Reverse);
		}
	}
}

UInventorySlot_WS* UInventory_WS::GetEmptySlot()
{
	UInventorySlot_WS* FindSlot = nullptr;

	for (auto Slot : InventorySlots)
	{
		if (Slot->GetIsEmpty())
		{
			FindSlot = Slot;
			break;
		}
	}

	return FindSlot;
}

UInventorySlot_WS* UInventory_WS::GetSlot(int index)
{
	if (index < 0 || index >= 8)
		return nullptr;

	return InventorySlots[index];
}

int UInventory_WS::GetSlotIndex(UInventorySlot_WS* InventorySlot)
{
	return InventorySlots.Find(InventorySlot);
}