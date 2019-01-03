// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/PracticeUMGWidget.h"
#include "TestDragPanel_WS.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_UMG_API UTestDragPanel_WS : public UPracticeUMGWidget
{
	GENERATED_BODY()
	
	
	
public:
	static UClass* GetWidgetClassStatic();
	virtual UClass* GetWidgetClass() override { return GetWidgetClassStatic(); }

	bool Initialize();

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};
