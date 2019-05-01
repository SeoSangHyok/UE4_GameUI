// Fill out your copyright notice in the Description page of Project Settings.

#include "Exit_WS.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/Engine/Classes/Engine/ObjectLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"



void UExit_WS::NativeConstruct()
{
	Super::NativeConstruct();

	UWidgetAnimation** FIndAnimation = AnimationList.Find(FName(TEXT("Showing")));
	if (nullptr != FIndAnimation)
	{
		PlayAnimation(*FIndAnimation);
	}
}

UClass* UExit_WS::GetWidgetClassStatic()
{
	FSoftObjectPath AssetLoader(TEXT("WidgetBlueprint'/Game/UMG/Exit_WB.Exit_WB_C'"));
	UClass* Object = Cast<UClass>(AssetLoader.TryLoad());
	if (!IsValid(Object))
		return nullptr;

	return Object;
}

bool UExit_WS::Initialize()
{
	Super::Initialize();

	YesButton->OnClicked.AddDynamic(this, &UExit_WS::OnClickedYesButton);
	NoButton->OnClicked.AddDynamic(this, &UExit_WS::OnClickedNoButton);

	return true;
}

void UExit_WS::OnClickedYesButton()
{
	UKismetSystemLibrary::QuitGame(GetOwningPlayer()->GetWorld(), GetOwningPlayer(), EQuitPreference::Type::Quit, true);
}

void UExit_WS::OnClickedNoButton()
{
	RemoveFromParent();
}