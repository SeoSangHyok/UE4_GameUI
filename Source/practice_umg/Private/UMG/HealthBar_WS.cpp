// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthBar_WS.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"



UClass* UHealthBar_WS::GetWidgetClassStatic()
{
	FSoftObjectPath AssetLoader(TEXT("WidgetBlueprint'/Game/UMG/HealthBar_WB.HealthBar_WB'_C'"));
	UClass* Object = Cast<UClass>(AssetLoader.TryLoad());
	if (!IsValid(Object))
		return nullptr;

	return Object;
}

void UHealthBar_WS::SetHPRate(float HpRate)
{
	HpRate = FMath::Clamp<float>(HpRate, 0.0f, 1.0f);

	HealthBar->SetPercent(HpRate);
}