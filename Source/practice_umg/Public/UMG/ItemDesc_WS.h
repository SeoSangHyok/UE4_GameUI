// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/PracticeUMGWidget.h"
#include "ItemDesc_WS.generated.h"

/**
 * 
 */
class UInventoryItem;

UCLASS()
class PRACTICE_UMG_API UItemDesc_WS : public UPracticeUMGWidget
{
	GENERATED_BODY()
	
	
public:
	static UClass* GetWidgetClassStatic();
	virtual UClass* GetWidgetClass() override { return GetWidgetClassStatic(); }

	virtual bool Initialize() override;

	void PlayShowAnimation();
	void SetDesc(UInventoryItem* InventoryItem);

private:
	UPROPERTY(meta = (Bindwidget))
	class UImage* ThumbName;

	UPROPERTY(meta = (Bindwidget))
	class UTextBlock* ItemName;

	UPROPERTY(meta = (Bindwidget))
	class UTextBlock* ItemDesc;
};
