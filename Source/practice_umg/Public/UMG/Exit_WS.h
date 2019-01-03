// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/PracticeUMGWidget.h"
#include "Exit_WS.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_UMG_API UExit_WS : public UPracticeUMGWidget
{
	GENERATED_BODY()
	
	
protected:
	virtual void NativeConstruct() override;
	virtual UClass* GetWidgetClass() override { return GetWidgetClassStatic(); }

public:
	static UClass* GetWidgetClassStatic();	

	virtual bool Initialize() override;

	UFUNCTION()
	void OnClickedYesButton();

	UFUNCTION()
	void OnClickedNoButton();



public:
	UPROPERTY(meta = (Bindwidget))
	class UButton* NoButton;

	UPROPERTY(meta = (Bindwidget))
	class UButton* YesButton;
};
