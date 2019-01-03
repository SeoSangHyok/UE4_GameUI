// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/PracticeUMGWidget.h"
#include "Contextmenu_WS.generated.h"

/**
 * 
 */
class UButton;
class UTextBlock;
class UMenuAnchor;
class UInventorySlot_WS;

UCLASS()
class PRACTICE_UMG_API UContextmenu_WS : public UPracticeUMGWidget
{
	GENERATED_BODY()

public:
	static UClass* GetWidgetClassStatic();
	virtual UClass* GetWidgetClass() override { return GetWidgetClassStatic(); }

	virtual bool Initialize() override;
	

	UFUNCTION()
	void OnClickedUseButton();

	UFUNCTION()
	void OnClickedThrowButton();

	UFUNCTION()
	void OnClickedCloseButton();

public:
	UPROPERTY()
	UMenuAnchor* MenuAnchor;

	UPROPERTY()
	UInventorySlot_WS* InventorySlot_WS;

public:
	UPROPERTY(meta = (Bindwidget))
	UTextBlock* UseText;

private:
	UPROPERTY(meta = (Bindwidget))
	UButton* UseButton;

	UPROPERTY(meta = (Bindwidget))
	UButton* ThrowButton;

	UPROPERTY(meta = (Bindwidget))
	UButton* CloseButton;
};
