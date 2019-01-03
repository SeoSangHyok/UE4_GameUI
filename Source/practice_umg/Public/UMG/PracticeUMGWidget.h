// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PracticeUMGWidget.generated.h"

/**
 * 
 */
class UWidgetAnimation;

UCLASS()
class PRACTICE_UMG_API UPracticeUMGWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
protected:
	virtual void NativeConstruct() override;

public:
	virtual UClass* GetWidgetClass() { return nullptr; }

protected:
	UPROPERTY()
	TMap<FName, UWidgetAnimation*> AnimationList;
};
