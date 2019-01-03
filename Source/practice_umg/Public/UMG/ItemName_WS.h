// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UMG/PracticeUMGWidget.h"
#include "ItemName_WS.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemNameButtonClicked);

class UWidgetAnimation;

UCLASS()
class PRACTICE_UMG_API UItemName_WS : public UPracticeUMGWidget
{
	GENERATED_BODY()
	
	
public:
	static UClass* GetWidgetClassStatic();
	virtual UClass* GetWidgetClass() override { return GetWidgetClassStatic(); }

	virtual bool Initialize() override;

	void SetItemName(FString NewItemName);

	void Showing();
	void Hiding();

	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

	UFUNCTION()
	void OnClickedButton();

	UFUNCTION()
	void OnHoveredButton();

	UFUNCTION()
	void OnUnHoveredButton();

public:
	UPROPERTY(meta = (Bindwidget))
	class UButton* ItemButton;

	UPROPERTY(meta = (Bindwidget))
	class UTextBlock* ItemName;

	UPROPERTY(meta = (Bindwidget))
	class UImage* BackImage;

	UPROPERTY()
	FOnItemNameButtonClicked OnItemNameButtonClicked;
};
