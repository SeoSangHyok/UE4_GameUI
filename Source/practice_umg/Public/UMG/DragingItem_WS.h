// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/PracticeUMGWidget.h"
#include "DragingItem_WS.generated.h"

/**
 * 
 */
class UImage;

UCLASS()
class PRACTICE_UMG_API UDragingItem_WS : public UPracticeUMGWidget
{
	GENERATED_BODY()
	
	
	
	virtual UClass* GetWidgetClass() override { return GetWidgetClassStatic(); }

public:
	static UClass* GetWidgetClassStatic();

	void SetItemImage(const FSoftObjectPath& ItemPathRef);

public:
	UPROPERTY(meta = (Bindwidget))
	UImage* ItemImage;
};
