// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/PracticeUMGWidget.h"
#include "InventorySlot_WS.generated.h"

/**
 * 
 */
class UImage;
class UTextBlock;
class UButton;
class UMenuAnchor;
class UInventoryItem;
class UItemDesc_WS;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCliekedInventorySlot, UInventoryItem*, InventoryItem, UInventorySlot_WS*, InventorySlot);

UCLASS()
class PRACTICE_UMG_API UInventorySlot_WS : public UPracticeUMGWidget
{
	GENERATED_BODY()
	
	
public:
	static UClass* GetWidgetClassStatic();
	virtual UClass* GetWidgetClass() override { return GetWidgetClassStatic(); }

	virtual bool Initialize() override;

	void SetItemInfo(UInventoryItem* NewInventoryItemRef);
	void ShowAnimation();
	bool GetIsEmpty() { return IsEmpty; }

	UFUNCTION()
	void OnSlotButtonCliecked();

	UFUNCTION()
	void CleanUp();

	UFUNCTION()
	void UpdateSlot();

	UFUNCTION()
	void OnContextUse();

	UFUNCTION()
	void OnContextThrow();

	UFUNCTION()
	void OnContextClose();

	UFUNCTION()
	UWidget* OnGetMenuContentEvent();

	UFUNCTION()
	void OnLButtonClick();

	UFUNCTION()
	void OnRButtonClick();

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	void ClearItemDesc();

public:
	UPROPERTY(meta = (Bindwidget))
	UImage* ItemImage;

	UPROPERTY(meta = (Bindwidget))
	UTextBlock* ItemStateText;

	UPROPERTY(meta = (Bindwidget))
	UMenuAnchor* ContextMenuAnchor;

	UPROPERTY()
	UInventoryItem* InventoryItemRef;

	UPROPERTY()
	FOnCliekedInventorySlot OnCliekedInventorySlot;

	UPROPERTY()
	FOnCliekedInventorySlot OnCliekedInventoryContext;

	FTimerHandle TimerHandle;

public:
	int SlotIndex = INDEX_NONE;

private:
	bool IsEmpty = true;
	bool LButtonDown = false;
	bool RButtonDown = false;
};
