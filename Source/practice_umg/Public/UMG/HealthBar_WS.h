// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/PracticeUMGWidget.h"
#include "HealthBar_WS.generated.h"

/**
 * 
 */
class UProgressBar;

UCLASS()
class PRACTICE_UMG_API UHealthBar_WS : public UPracticeUMGWidget
{
	GENERATED_BODY()
	
public:
	static UClass* GetWidgetClassStatic();
	virtual UClass* GetWidgetClass() override { return GetWidgetClassStatic(); }
	
	void SetHPRate(float HpRate);

private:
	UPROPERTY(meta = (Bindwidget))
	UProgressBar* HealthBar;
};
