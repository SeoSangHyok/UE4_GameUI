// Fill out your copyright notice in the Description page of Project Settings.

#include "TestDragPanel_WS.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Runtime/UMG/Public/Blueprint/SlateBlueprintLibrary.h"
#include "UMG/WidgetDragDrop.h"



UClass* UTestDragPanel_WS::GetWidgetClassStatic()
{
	FSoftObjectPath AssetLoader(TEXT("WidgetBlueprint'/Game/UMG/TestDragPanel_WB.TestDragPanel_WB_C'"));
	UClass* Object = Cast<UClass>(AssetLoader.TryLoad());
	if (!IsValid(Object))
		return nullptr;

	return Object;
}

bool UTestDragPanel_WS::Initialize()
{
	Super::Initialize();

	return true;
}

FReply UTestDragPanel_WS::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UTestDragPanel_WS::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UWidgetDragDrop* WidgetDragDropOperation = Cast<UWidgetDragDrop>(UWidgetBlueprintLibrary::CreateDragDropOperation(UWidgetDragDrop::StaticClass()));
	WidgetDragDropOperation->TargetWidgetRef = this;
	WidgetDragDropOperation->DefaultDragVisual = this;

 	FVector2D MouseScreenPos = InMouseEvent.GetScreenSpacePosition();
 	FVector2D MouseLocalOffset = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry, MouseScreenPos);
 	WidgetDragDropOperation->TargetOffset = MouseLocalOffset;

	WidgetDragDropOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = WidgetDragDropOperation;
	
	RemoveFromParent();
}