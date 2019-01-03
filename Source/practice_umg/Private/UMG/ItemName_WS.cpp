// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemName_WS.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"


UClass* UItemName_WS::GetWidgetClassStatic()
{
	FSoftObjectPath AssetLoader(TEXT("WidgetBlueprint'/Game/UMG/ItemName_WB.ItemName_WB_C'"));
	UClass* Object = Cast<UClass>(AssetLoader.TryLoad());
	if (!IsValid(Object))
		return nullptr;

	return Object;
}

bool UItemName_WS::Initialize()
{
	Super::Initialize();

	ItemButton->OnClicked.AddDynamic(this, &UItemName_WS::OnClickedButton);
	ItemButton->OnHovered.AddDynamic(this, &UItemName_WS::OnHoveredButton);
	ItemButton->OnUnhovered.AddDynamic(this, &UItemName_WS::OnUnHoveredButton);

	return true;
}

void UItemName_WS::SetItemName(FString NewItemName)
{
	//ItemName->SetText(FText::AsCultureInvariant(NewItemName));
	ItemName->SetText(FText::FromString(NewItemName));
}

void UItemName_WS::Showing()
{
	UWidgetAnimation** WidgetAnimation = AnimationList.Find(FName(TEXT("Showing")));
	if (nullptr != WidgetAnimation)
	{
		PlayAnimation(*WidgetAnimation);
	}
}

void UItemName_WS::Hiding()
{
	UWidgetAnimation** WidgetAnimation = AnimationList.Find(FName(TEXT("Hiding")));
	if (nullptr != WidgetAnimation)
	{
		PlayAnimation(*WidgetAnimation);
	}
}

void UItemName_WS::OnClickedButton()
{
	if (OnItemNameButtonClicked.IsBound())
	{
		OnItemNameButtonClicked.Broadcast();
	}
}

void UItemName_WS::OnHoveredButton()
{
	BackImage->SetOpacity(0.8f);
}

void UItemName_WS::OnUnHoveredButton()
{
	BackImage->SetOpacity(0.5f);
}

void UItemName_WS::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (IsValid(Animation) && Animation->GetFName() == FName(TEXT("Hiding")))
	{
		RemoveFromParent();
	}
}