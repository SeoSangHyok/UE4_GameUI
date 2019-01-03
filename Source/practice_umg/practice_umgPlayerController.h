// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Item/InventoryItem.h"
#include "practice_umgPlayerController.generated.h"

enum class EItemEquipSlot : uint8;

//class UInventoryItem;
class UInventorySlot_WS;

UCLASS()
class Apractice_umgPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	Apractice_umgPlayerController();

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void BeginPlay() override;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

public:
	UFUNCTION()
	void IP_Esc();

	UFUNCTION()
	void IP_Inventory();

	bool TryPickingItem(int ItemClassID, int StackCount);

	void ExchangeInventorySlot(UInventoryItem* SrcInventoryItem, UInventorySlot_WS* SrcInventorySlot, UInventoryItem* DestInventoryItem, UInventorySlot_WS* DestInventorySlot);

	void SplitInventoryItem(UInventoryItem* SrcInventoryItem, UInventorySlot_WS* SrcInventorySlot, UInventoryItem* DestInventoryItem, UInventorySlot_WS* DestInventorySlot);

	UFUNCTION()
	void OnclieckedInventorySlot(UInventoryItem* InventoryItem, UInventorySlot_WS* InventorySlot);

	UFUNCTION()
	void OnclieckedContextThrow(UInventoryItem* InventoryItem, UInventorySlot_WS* InventorySlot);

	UFUNCTION()
	void GetEquipedInventoryItem(EItemEquipSlot ItemEquipSlot, UInventoryItem* &InventoryItem, UInventorySlot_WS* &InventorySlot);

	UFUNCTION()
	int GetEmptyInvenItem(UInventoryItem*& FoundSlot);

	UFUNCTION()
	int FindInventoryItemByItemID(int ItemClassID, UInventoryItem*& FoundSlot);

public:
	UPROPERTY()
	class UMainContainer_WS* WidgetMainContainer;

	UPROPERTY()
	TArray<UInventoryItem*> InventoryItems;	

private:
	const int MaxInventorySlotCount = 8;
};


