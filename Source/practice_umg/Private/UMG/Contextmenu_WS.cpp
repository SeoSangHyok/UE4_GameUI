// Fill out your copyright notice in the Description page of Project Settings.

#include "Contextmenu_WS.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/MenuAnchor.h"
#include "UMG/InventorySlot_WS.h"



UClass* UContextmenu_WS::GetWidgetClassStatic()
{
	FSoftObjectPath AssetLoader(TEXT("WidgetBlueprint'/Game/UMG/ContextMenu_WB.ContextMenu_WB_C'"));
	UClass* Object = Cast<UClass>(AssetLoader.TryLoad());
	if (!IsValid(Object))
		return nullptr;

	return Object;
}

bool UContextmenu_WS::Initialize()
{
	Super::Initialize();

	if (IsValid(UseButton))
		UseButton->OnClicked.AddDynamic(this, &UContextmenu_WS::OnClickedUseButton);

	if (IsValid(ThrowButton))
		ThrowButton->OnClicked.AddDynamic(this, &UContextmenu_WS::OnClickedThrowButton);

	if (IsValid(CloseButton))
		CloseButton->OnClicked.AddDynamic(this, &UContextmenu_WS::OnClickedCloseButton);

	return true;
}

void UContextmenu_WS::OnClickedUseButton()
{
	if (IsValid(MenuAnchor))
		MenuAnchor->Close();

	InventorySlot_WS->OnContextUse();
}

void UContextmenu_WS::OnClickedThrowButton()
{
	if (IsValid(MenuAnchor))
		MenuAnchor->Close();

	InventorySlot_WS->OnContextThrow();
}

void UContextmenu_WS::OnClickedCloseButton()
{
	if (IsValid(MenuAnchor))
		MenuAnchor->Close();

	InventorySlot_WS->OnContextClose();
}