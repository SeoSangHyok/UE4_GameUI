// Fill out your copyright notice in the Description page of Project Settings.

#include "MainContainer_WS.h"
#include "Runtime/UMG/Public/Components/CanvasPanel.h"
#include "Runtime/UMG/Public/Components/CanvasPanelSlot.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/UMG/Public/Blueprint/SlateBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Engine/ObjectLibrary.h"
#include "Runtime/Core/Public/Misc/MessageDialog.h"
#include "UMG/Exit_WS.h"
#include "UMG/Inventory_WS.h"
#include "UMG/ItemDesc_WS.h"
#include "UMG/WidgetDragDrop.h"
#include "UMG/TestDragPanel_WS.h"
#include "UMG/ItemDragDrop.h"
#include "UMG/InventorySlot_WS.h"
#include "Item/InventoryItem.h"
#include "practice_umgPlayerController.h"



UClass* UMainContainer_WS::GetWidgetClassStatic()
{
	FSoftObjectPath AssetLoader(TEXT("WidgetBlueprint'/Game/UMG/MainContainer_WB.MainContainer_WB_C'"));
	UClass* Object = Cast<UClass>(AssetLoader.TryLoad());
	if (!IsValid(Object))
		return nullptr;

	return Object;

// 	FSoftClassPath WidgetClassLoader(TEXT("WidgetBlueprint'/Game/UMG/MainContainer_WB.MainContainer_WB_C'"));
// 	UClass* WidgetClass = WidgetClassLoader.TryLoadClass<UUserWidget>();
// 	return WidgetClass;
}

bool UMainContainer_WS::Initialize()
{
	Super::Initialize();

	if (IsValid(ItemDesc))
	{
		ItemDesc->SetVisibility(ESlateVisibility::Collapsed);
		UCanvasPanelSlot* ItemDescCanvasSlot = Cast<UCanvasPanelSlot>(ItemDesc->Slot);
		if (IsValid(ItemDescCanvasSlot))
		{
			ItemDescCanvasSlot->SetZOrder(255);
		}
	}

	SetVisibility(ESlateVisibility::Visible);

	return true;
}

bool UMainContainer_WS::OpenWidget(UUserWidget* Widget, FAnchors Anchors, FVector2D Alignment, FVector2D Position)
{
	if (!IsValid(MainContainer) || !IsValid(Widget))
	{
		return false;
	}

	UCanvasPanelSlot* CanvasPanelSlot = MainContainer->AddChildToCanvas(Widget);
	if (!IsValid(CanvasPanelSlot))
		return false;

	CanvasPanelSlot->SetAutoSize(true);
	CanvasPanelSlot->SetAlignment(Alignment);
	CanvasPanelSlot->SetAnchors(Anchors);
	CanvasPanelSlot->SetPosition(Position);


	return true;
}

bool UMainContainer_WS::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

 	UItemDragDrop* ItemDragDropOperation = Cast<UItemDragDrop>(InOperation);
	if (!IsValid(ItemDragDropOperation))
		return false;

	Apractice_umgPlayerController* practice_umgPlayerController = Cast<Apractice_umgPlayerController>(GetOwningPlayer());
	if (!IsValid(practice_umgPlayerController))
		return false;

	practice_umgPlayerController->OnclieckedContextThrow(ItemDragDropOperation->InventoryItemRef, ItemDragDropOperation->FromInventorySlotRef);

	return true;
}