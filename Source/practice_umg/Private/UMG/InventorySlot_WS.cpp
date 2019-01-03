// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlot_WS.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/MenuAnchor.h"
#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"
#include "Runtime/UMG/Public/Blueprint/SlateBlueprintLibrary.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Runtime/UMG/Public/Components/CanvasPanelSlot.h"
#include "Runtime/SlateCore/Public/Input/Reply.h"
#include "practice_umg_GameInstance.h"
#include "TableManager.h"
#include "Item/InventoryItem.h"
#include "Item/ItemTableDef.h"
#include "practice_umgPlayerController.h"
#include "UMG/Contextmenu_WS.h"
#include "UMG/ItemDesc_WS.h"
#include "UMG/MainContainer_WS.h"
#include "UMG/ItemDragDrop.h"
#include "UMG/DragingItem_WS.h"


UClass* UInventorySlot_WS::GetWidgetClassStatic()
{
	FSoftObjectPath AssetLoader(TEXT("WidgetBlueprint'/Game/UMG/InventorySlot_WB.InventorySlot_WB_C'"));
	UClass* Object = Cast<UClass>(AssetLoader.TryLoad());
	if (!IsValid(Object))
		return nullptr;

	return Object;
}

bool UInventorySlot_WS::Initialize()
{
	Super::Initialize();

	if (IsValid(ContextMenuAnchor))
	{
		ContextMenuAnchor->OnGetMenuContentEvent.BindUFunction(this, FName("OnGetMenuContentEvent"));
	}

	SetItemInfo(0);

	return true;
}

void UInventorySlot_WS::SetItemInfo(UInventoryItem* NewInventoryItemRef)
{
	InventoryItemRef = NewInventoryItemRef;
	if (nullptr == NewInventoryItemRef)
	{
		CleanUp();
		return;
	}

	const FItemTableRow* ItemTable = InventoryItemRef->GetItemTableRow();

	if (nullptr == ItemTable)
	{
		CleanUp();
		return;
	}

	IsEmpty = false;
	ItemImage->SetBrushFromTexture(Cast<UTexture2D>(ItemTable->ItemThumb.TryLoad()), true);
	ItemImage->SetVisibility(ESlateVisibility::Visible);
}

void UInventorySlot_WS::ShowAnimation()
{
	UWidgetAnimation** WidgetAnimation = AnimationList.Find(FName(TEXT("ShowingItemImage")));
	if (nullptr != WidgetAnimation)
	{
		PlayAnimation(*WidgetAnimation);
	}
}

void UInventorySlot_WS::OnSlotButtonCliecked()
{
	if (OnCliekedInventorySlot.IsBound())
	{
		OnCliekedInventorySlot.Broadcast(InventoryItemRef, this);
	}
}

void UInventorySlot_WS::CleanUp()
{
	ClearItemDesc();
	OnCliekedInventorySlot.Clear();
	OnCliekedInventoryContext.Clear();
	IsEmpty = true;
	ItemImage->SetBrushFromTexture(nullptr);
	ItemImage->SetVisibility(ESlateVisibility::Collapsed);
	ItemStateText->SetText(FText::FromString(TEXT("")));
	InventoryItemRef = nullptr;
}

void UInventorySlot_WS::UpdateSlot()
{
	if (!IsValid(InventoryItemRef))
	{
		CleanUp();
		return;
	}

	const FItemTableRow* ItemTableRow = InventoryItemRef->GetItemTableRow();
	if (nullptr == ItemTableRow)
		return;

	if (ItemTableRow->ItemType == EItemType::Consume)
	{
		if (InventoryItemRef->StackCount <= 1)
		{
			ItemStateText->SetText(FText::FromString(TEXT("")));
		}
		else
		{
			ItemStateText->SetText(FText::FromString(FString::FromInt(InventoryItemRef->StackCount)));
		}
	}
	else
	{
		if (InventoryItemRef->Equipped)
		{
			FString EquipText(TEXT("Equiped"));
			ItemStateText->SetText(FText::FromString(EquipText));
		}
		else
		{
			FString EquipText(TEXT(""));
			ItemStateText->SetText(FText::FromString(EquipText));
		}
	}
}

void UInventorySlot_WS::OnContextUse()
{
	if (OnCliekedInventorySlot.IsBound())
	{
		OnCliekedInventorySlot.Broadcast(InventoryItemRef, this);
	}
}

void UInventorySlot_WS::OnContextThrow()
{
	if (OnCliekedInventoryContext.IsBound())
	{
		OnCliekedInventoryContext.Broadcast(InventoryItemRef, this);
	}
}

void UInventorySlot_WS::OnContextClose()
{

}

FReply UInventorySlot_WS::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	LButtonDown = InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton);
	RButtonDown = (false == LButtonDown) && InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton);

	if (LButtonDown)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}	

	return FReply::Handled();
}

FReply UInventorySlot_WS::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if (LButtonDown)
	{
		OnLButtonClick();
	} else if (RButtonDown)
	{
		OnRButtonClick();
	}
	else
	{
		return FReply::Unhandled();
	}

	LButtonDown = false;
	RButtonDown = false;

	return FReply::Handled();
}

FReply UInventorySlot_WS::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);

	return FReply::Handled();
}

void UInventorySlot_WS::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (true == IsEmpty)
		return;

	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([=]
	{
		Apractice_umgPlayerController* practice_umgPlayerController = Cast<Apractice_umgPlayerController>(GetOwningPlayer());
		if (!IsValid(practice_umgPlayerController))
			return;

		UItemDesc_WS* ItemDesc_WS = practice_umgPlayerController->WidgetMainContainer->ItemDesc;
		if (IsValid(ItemDesc_WS))
		{
			ItemDesc_WS->SetVisibility(ESlateVisibility::HitTestInvisible/* ::Visible*/);

			FVector2D ViewportPixelPos;
			FVector2D ViewportPos;
			USlateBlueprintLibrary::AbsoluteToViewport(practice_umgPlayerController->GetWorld(), InMouseEvent.GetScreenSpacePosition(), ViewportPixelPos, ViewportPos);

			UCanvasPanelSlot* ItemDescSlot = Cast<UCanvasPanelSlot>(ItemDesc_WS->Slot);
			if (IsValid(ItemDescSlot))
			{
				ItemDescSlot->SetAlignment(FVector2D(0.5f, 1.0f));
				ViewportPos.Y += -10.0f;
				ItemDescSlot->SetPosition(ViewportPos);
				ItemDesc_WS->SetDesc(InventoryItemRef);
				ItemDesc_WS->PlayShowAnimation();
			}
		}
	});

	FTimerManager& TimerManager = GetOwningPlayer()->GetWorldTimerManager();
	if (TimerManager.TimerExists(TimerHandle))
	{
		TimerManager.ClearTimer(TimerHandle);
	}
	
	TimerManager.SetTimer(TimerHandle, TimerCallback, 0.5f, false);
}

void UInventorySlot_WS::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	ClearItemDesc();
}

FReply UInventorySlot_WS::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseMove(InGeometry, InMouseEvent);

	return FReply::Unhandled();
}

void UInventorySlot_WS::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UItemDragDrop* ItemDragDropOperation = Cast<UItemDragDrop>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemDragDrop::StaticClass()));
	if (!IsValid(ItemDragDropOperation))
		return;

	UDragingItem_WS* DragingItemWidget = CreateWidget<UDragingItem_WS>(GetOwningPlayer(), UDragingItem_WS::GetWidgetClassStatic());
	if (IsValid(DragingItemWidget))
	{
		const FItemTableRow* ItemTableRow = InventoryItemRef->GetItemTableRow();
		if (nullptr != ItemTableRow)
		{
			DragingItemWidget->SetItemImage(ItemTableRow->ItemThumb);
		}

		ItemDragDropOperation->DefaultDragVisual = DragingItemWidget;
	}

	ItemDragDropOperation->Split = InMouseEvent.IsShiftDown();
	ItemDragDropOperation->FromInventorySlotRef = this;
	ItemDragDropOperation->InventoryItemRef = InventoryItemRef;
	ItemDragDropOperation->Pivot = EDragPivot::MouseDown;
	
	OutOperation = ItemDragDropOperation;

	LButtonDown = false;
	RButtonDown = false;
}

void UInventorySlot_WS::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	LButtonDown = false;
	RButtonDown = false;
}

bool UInventorySlot_WS::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemDragDrop* ItemDragDropOperation = Cast<UItemDragDrop>(InOperation);
	if (!IsValid(ItemDragDropOperation))
		return false;

	if (this == ItemDragDropOperation->FromInventorySlotRef)
	{
		return true;
	}

	Apractice_umgPlayerController* practice_umgPlayerController = Cast<Apractice_umgPlayerController>(GetOwningPlayer());
	if (!IsValid(practice_umgPlayerController))
		return false;

	if (ItemDragDropOperation->Split)
	{
		practice_umgPlayerController->SplitInventoryItem(ItemDragDropOperation->InventoryItemRef, ItemDragDropOperation->FromInventorySlotRef, InventoryItemRef, this);
	}
	else
	{
		practice_umgPlayerController->ExchangeInventorySlot(ItemDragDropOperation->InventoryItemRef, ItemDragDropOperation->FromInventorySlotRef, InventoryItemRef, this);
	}

	ItemImage->SetOpacity(1.0f);


	return true;
}

void UInventorySlot_WS::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	UItemDragDrop* ItemDragDropOperation = Cast<UItemDragDrop>(InOperation);
	if (!IsValid(ItemDragDropOperation))
		return;

	if (false == IsEmpty && this != ItemDragDropOperation->FromInventorySlotRef)
		ItemImage->SetOpacity(0.5f);
}

void UInventorySlot_WS::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	UItemDragDrop* ItemDragDropOperation = Cast<UItemDragDrop>(InOperation);
	if (!IsValid(ItemDragDropOperation))
		return;

	ItemImage->SetOpacity(1.0f);
}

void UInventorySlot_WS::ClearItemDesc()
{
	Apractice_umgPlayerController* practice_umgPlayerController = Cast<Apractice_umgPlayerController>(GetOwningPlayer());
	if (!IsValid(practice_umgPlayerController))
		return;

	UItemDesc_WS* ItemDesc_WS = practice_umgPlayerController->WidgetMainContainer->ItemDesc;
	if (IsValid(ItemDesc_WS))
	{
		ItemDesc_WS->SetVisibility(ESlateVisibility::Collapsed);
	}

	FTimerManager& TimerManager = practice_umgPlayerController->GetWorldTimerManager();
	if (TimerManager.TimerExists(TimerHandle) && TimerHandle.IsValid())
	{
		TimerManager.ClearTimer(TimerHandle);
	}
}

UWidget* UInventorySlot_WS::OnGetMenuContentEvent()
{
	if (!IsValid(InventoryItemRef))
		return nullptr;

	UContextmenu_WS* Contextmenu = CreateWidget<UContextmenu_WS>(GetOwningPlayer(), UContextmenu_WS::GetWidgetClassStatic());

	Contextmenu->MenuAnchor = ContextMenuAnchor;
	Contextmenu->InventorySlot_WS = this;


	const FItemTableRow* ItemTableRow = InventoryItemRef->GetItemTableRow();
	if (ItemTableRow)
	{
		FString UseString;
		switch (ItemTableRow->ItemType)
		{
		case EItemType::Consume:
			UseString = TEXT("Consume");
			Contextmenu->UseText->SetText(FText::FromString(UseString));
			break;

		case EItemType::Equip:
			if (InventoryItemRef->Equipped)
			{
				UseString = TEXT("UnEquip");
				Contextmenu->UseText->SetText(FText::FromString(UseString));
			}
			else
			{
				UseString = TEXT("Equip");
				Contextmenu->UseText->SetText(FText::FromString(UseString));
			}
			break;

		default:
			UseString = TEXT("Use");
			Contextmenu->UseText->SetText(FText::FromString(UseString));
			break;
		}
	}

	return Contextmenu;
}

void UInventorySlot_WS::OnLButtonClick()
{
	if (OnCliekedInventorySlot.IsBound())
	{
		OnCliekedInventorySlot.Broadcast(InventoryItemRef, this);
	}
}

void UInventorySlot_WS::OnRButtonClick()
{
	if (true == IsEmpty)
		return;

	if (IsValid(ContextMenuAnchor))
	{
		ContextMenuAnchor->Open(true);
	}
}