// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Slate/Public/Widgets/Layout/Anchors.h"
#include "UMG/PracticeUMGWidget.h"
#include "MainContainer_WS.generated.h"

/**
 * 
 */
class UButton;
class UInventory_WS;
class UHealthBar_WS;
class UItemDesc_WS;

UCLASS()
class PRACTICE_UMG_API UMainContainer_WS : public UPracticeUMGWidget
{
	GENERATED_BODY()
	
public:
	static UClass* GetWidgetClassStatic();
	virtual UClass* GetWidgetClass() override { return GetWidgetClassStatic(); }

	virtual bool Initialize() override;

	bool OpenWidget(UUserWidget* Widget, FAnchors Anchors, FVector2D Alignment, FVector2D Position);

protected:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	UPROPERTY(meta = (Bindwidget))
	class UCanvasPanel* MainContainer;

	UPROPERTY(meta = (Bindwidget))
	class UInventory_WS* Inventory;

	UPROPERTY(meta = (Bindwidget))
	class UHealthBar_WS* HealthBar;

	UPROPERTY(meta = (Bindwidget))
	UItemDesc_WS* ItemDesc;
};

