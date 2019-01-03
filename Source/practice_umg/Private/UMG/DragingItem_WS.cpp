// Fill out your copyright notice in the Description page of Project Settings.

#include "DragingItem_WS.h"
#include "Runtime/UMG/Public/Components/Image.h"



UClass* UDragingItem_WS::GetWidgetClassStatic()
{
	FSoftObjectPath AssetLoader(TEXT("WidgetBlueprint'/Game/UMG/DragingItem_WB.DragingItem_WB_C'"));
	UClass* Object = Cast<UClass>(AssetLoader.TryLoad());
	if (!IsValid(Object))
		return nullptr;

	return Object;
}

void UDragingItem_WS::SetItemImage(const FSoftObjectPath& ItemPathRef)
{
	if (IsValid(ItemImage))
	{
		ItemImage->SetBrushFromTexture(Cast<UTexture2D>(ItemPathRef.TryLoad()));
	}
}