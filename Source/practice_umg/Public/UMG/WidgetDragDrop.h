// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "WidgetDragDrop.generated.h"

/**
 * 
 */
class UTestDragPanel_WS;

UCLASS()
class PRACTICE_UMG_API UWidgetDragDrop : public UDragDropOperation
{
	GENERATED_BODY()




public:
	UPROPERTY()
	UTestDragPanel_WS* TargetWidgetRef;

	UPROPERTY()
	FVector2D TargetOffset;
};
