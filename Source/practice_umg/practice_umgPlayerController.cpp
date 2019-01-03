// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "practice_umgPlayerController.h"
#include "practice_umg_GameInstance.h"
#include "TableManager.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "practice_umgCharacter.h"
#include "Engine/World.h"
#include "UMG/MainContainer_WS.h"
#include "UMG/Exit_WS.h"
#include "UMG/Inventory_WS.h"
#include "UMG/InventorySlot_WS.h"
#include "UMG/HealthBar_WS.h"
#include "Item/InventoryItem.h"
#include "Item/ItemTableDef.h"
#include "Item/Item.h"

Apractice_umgPlayerController::Apractice_umgPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;	
}

void Apractice_umgPlayerController::BeginPlay()
{
	Super::BeginPlay();


	WidgetMainContainer = CreateWidget<UMainContainer_WS>(this, UMainContainer_WS::GetWidgetClassStatic());
	if (IsValid(WidgetMainContainer) && IsLocalPlayerController())
	{
		WidgetMainContainer->AddToViewport(0);
	}

	InventoryItems.SetNumZeroed(MaxInventorySlotCount);

	Apractice_umgCharacter* practice_umgCharacter = Cast<Apractice_umgCharacter>(GetPawn());
	if (IsValid(practice_umgCharacter))
	{
		UHealthBar_WS* HealthBar_WS = WidgetMainContainer->HealthBar;
		if (IsValid(HealthBar_WS))
		{
			HealthBar_WS->SetHPRate(practice_umgCharacter->GetCurrentHPRate());
		}
	}
}

void Apractice_umgPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void Apractice_umgPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &Apractice_umgPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &Apractice_umgPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &Apractice_umgPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &Apractice_umgPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &Apractice_umgPlayerController::OnResetVR);

	InputComponent->BindKey(EKeys::X, EInputEvent::IE_Pressed, this, &Apractice_umgPlayerController::IP_Esc);

	InputComponent->BindKey(EKeys::I, EInputEvent::IE_Pressed, this, &Apractice_umgPlayerController::IP_Inventory);
}

void Apractice_umgPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void Apractice_umgPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (Apractice_umgCharacter* MyPawn = Cast<Apractice_umgCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UNavigationSystem::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void Apractice_umgPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void Apractice_umgPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f))
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void Apractice_umgPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void Apractice_umgPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}


void Apractice_umgPlayerController::IP_Esc()
{
	UExit_WS* Exit_WS = CreateWidget<UExit_WS>(this, UExit_WS::GetWidgetClassStatic());
	if (IsValid(Exit_WS))
	{
		WidgetMainContainer->OpenWidget(Exit_WS, FAnchors(0.5f, 0.5f, 0.5f, 0.5f), FVector2D(0.5f, 0.5f), FVector2D(0.0f, 0.0f));
	}	
}

void Apractice_umgPlayerController::IP_Inventory()
{
	UInventory_WS* Inventory = WidgetMainContainer->Inventory;
	if (!IsValid(Inventory))
		return;

	Inventory->ToggleInventory();
}

bool Apractice_umgPlayerController::TryPickingItem(int ItemClassID, int StackCount)
{
	UTableManager* TableManager = Upractice_umg_GameInstance::GetTableManager();
	if (!IsValid(TableManager) || 0 == ItemClassID)
	{
		return false;
	}

	const FItemTableRow* ItemTableRow = TableManager->GetTable<FItemTableRow>(ItemClassID);
	if (nullptr == ItemTableRow)
		return false;

	if (EItemType::Consume == ItemTableRow->ItemType)
	{
		UInventoryItem* TargetItem = nullptr;
		int TargetItemIndex = INDEX_NONE;
		bool AddNewSlot = true;

		TargetItemIndex = FindInventoryItemByItemID(ItemClassID, TargetItem);
		if (INDEX_NONE != TargetItemIndex && IsValid(TargetItem))
			AddNewSlot = false;
		else if (INDEX_NONE != (TargetItemIndex = GetEmptyInvenItem(TargetItem)) && IsValid(TargetItem))
			AddNewSlot = true;
		else
			return false;

		UInventory_WS* Inventory = WidgetMainContainer->Inventory;
		if (!IsValid(Inventory))
			return false;

		UInventorySlot_WS* InventorySlot = Inventory->GetSlot(TargetItemIndex);
		if (!IsValid(InventorySlot))
			return false;

		if (AddNewSlot)
		{
			TargetItem->SetItemInfo(ItemClassID, StackCount);
			InventorySlot->CleanUp();
			InventorySlot->SetItemInfo(TargetItem);
			InventorySlot->UpdateSlot();
			InventorySlot->OnCliekedInventorySlot.AddDynamic(this, &Apractice_umgPlayerController::OnclieckedInventorySlot);
			InventorySlot->OnCliekedInventoryContext.AddDynamic(this, &Apractice_umgPlayerController::OnclieckedContextThrow);

			if (Inventory->RenderOpacity == 1.0f)
			{
				InventorySlot->ShowAnimation();
			}
		}
		else
		{
			TargetItem->StackCount += StackCount;
			InventorySlot->SetItemInfo(TargetItem);
			InventorySlot->UpdateSlot();
		}
	}
	else
	{
		UInventoryItem* TargetItem = nullptr;
		int TargetItemIndex = INDEX_NONE;

		TargetItemIndex = GetEmptyInvenItem(TargetItem);
		if (INDEX_NONE == TargetItemIndex /*|| !IsValid(TargetItem)*/)
			return false;

		TargetItem->SetItemInfo(ItemClassID, StackCount);

		UInventory_WS* Inventory = WidgetMainContainer->Inventory;
		if (!IsValid(Inventory))
			return false;

		UInventorySlot_WS* InventorySlot = Inventory->GetSlot(TargetItemIndex);
		if (!IsValid(InventorySlot))
			return false;

		InventorySlot->CleanUp();
		InventorySlot->SetItemInfo(TargetItem);
		InventorySlot->UpdateSlot();
		InventorySlot->OnCliekedInventorySlot.AddDynamic(this, &Apractice_umgPlayerController::OnclieckedInventorySlot);
		InventorySlot->OnCliekedInventoryContext.AddDynamic(this, &Apractice_umgPlayerController::OnclieckedContextThrow);

		if (Inventory->RenderOpacity == 1.0f)
		{
			InventorySlot->ShowAnimation();
		}
	}

	return true;
}

void Apractice_umgPlayerController::ExchangeInventorySlot(UInventoryItem* SrcInventoryItem, UInventorySlot_WS* SrcInventorySlot, UInventoryItem* DestInventoryItem, UInventorySlot_WS* DestInventorySlot)
{
	if (!IsValid(SrcInventoryItem))
		return;

	int32 srcInvenIndex = InventoryItems.Find(SrcInventoryItem);
	if (INDEX_NONE == srcInvenIndex)
		return;

	int32 DestInvenIndex = INDEX_NONE;

	if (!IsValid(DestInventoryItem))
	{
		DestInvenIndex = DestInventorySlot->SlotIndex;
		if (INDEX_NONE == DestInvenIndex)
			return;

		InventoryItems[DestInvenIndex] = SrcInventoryItem;
		InventoryItems[srcInvenIndex] = nullptr;

		SrcInventorySlot->CleanUp();

		DestInventorySlot->CleanUp();
		DestInventorySlot->SetItemInfo(SrcInventoryItem);
		DestInventorySlot->UpdateSlot();
		DestInventorySlot->OnCliekedInventorySlot.AddDynamic(this, &Apractice_umgPlayerController::OnclieckedInventorySlot);
		DestInventorySlot->OnCliekedInventoryContext.AddDynamic(this, &Apractice_umgPlayerController::OnclieckedContextThrow);
	}
	else
	{
		if (SrcInventoryItem->ItemClassID == DestInventoryItem->ItemClassID && SrcInventoryItem->GetItemTableRow()->ItemType == EItemType::Consume)
		{
			DestInvenIndex = InventoryItems.Find(DestInventoryItem);
			if (INDEX_NONE == DestInvenIndex)
				return;

			DestInventoryItem->StackCount += SrcInventoryItem->StackCount;
			InventoryItems[srcInvenIndex] = nullptr;

			SrcInventorySlot->CleanUp();
			DestInventorySlot->UpdateSlot();
		}
		else
		{
			DestInvenIndex = InventoryItems.Find(DestInventoryItem);
			if (INDEX_NONE == DestInvenIndex)
				return;

			InventoryItems[DestInvenIndex] = SrcInventoryItem;
			InventoryItems[srcInvenIndex] = DestInventoryItem;

			SrcInventorySlot->SetItemInfo(DestInventoryItem);
			SrcInventorySlot->UpdateSlot();

			DestInventorySlot->SetItemInfo(SrcInventoryItem);
			DestInventorySlot->UpdateSlot();
		}
	}
}

void Apractice_umgPlayerController::SplitInventoryItem(UInventoryItem* SrcInventoryItem, UInventorySlot_WS* SrcInventorySlot, UInventoryItem* DestInventoryItem, UInventorySlot_WS* DestInventorySlot)
{
	if (!IsValid(SrcInventoryItem))
		return;

	int32 srcInvenIndex = InventoryItems.Find(SrcInventoryItem);
	if (INDEX_NONE == srcInvenIndex)
		return;

	int HalfStackCount = SrcInventoryItem->StackCount / 2.0f;
	if (0 == HalfStackCount)
		return;

	if (!IsValid(DestInventoryItem))
	{
		int DestInvenSlotIndex = DestInventorySlot->SlotIndex;
		if (INDEX_NONE == DestInvenSlotIndex)
			return;
		
		SrcInventoryItem->StackCount -= HalfStackCount;
		SrcInventorySlot->UpdateSlot();

		UInventoryItem* NewInventoryItem = NewObject<UInventoryItem>();
		NewInventoryItem->SetItemInfo(SrcInventoryItem->ItemClassID, HalfStackCount);
		InventoryItems[DestInvenSlotIndex] = NewInventoryItem;

		DestInventorySlot->CleanUp();
		DestInventorySlot->SetItemInfo(NewInventoryItem);
		DestInventorySlot->UpdateSlot();
		DestInventorySlot->OnCliekedInventorySlot.AddDynamic(this, &Apractice_umgPlayerController::OnclieckedInventorySlot);
		DestInventorySlot->OnCliekedInventoryContext.AddDynamic(this, &Apractice_umgPlayerController::OnclieckedContextThrow);
	}
	else
	{
		if (SrcInventoryItem->ItemClassID == DestInventoryItem->ItemClassID)
		{
			SrcInventoryItem->StackCount -= HalfStackCount;
			SrcInventorySlot->UpdateSlot();

			DestInventoryItem->StackCount += HalfStackCount;
			DestInventorySlot->UpdateSlot();
		}
		else
		{
			ExchangeInventorySlot(SrcInventoryItem, SrcInventorySlot, DestInventoryItem, DestInventorySlot);
		}
	}
}

void Apractice_umgPlayerController::OnclieckedInventorySlot(UInventoryItem* InventoryItem, UInventorySlot_WS* InventorySlot)
{
	if (!IsValid(InventorySlot) || !IsValid(InventorySlot->InventoryItemRef))
		return;

	int TargetItemIndex = InventoryItems.Find(InventorySlot->InventoryItemRef);
	if (INDEX_NONE == TargetItemIndex)
		return;

	UInventoryItem* TargetInventoryItem = InventoryItems[TargetItemIndex];
	const FItemTableRow* ItemTable = TargetInventoryItem->GetItemTableRow();
	if (nullptr == ItemTable)
		return;

	switch (ItemTable->ItemType)
	{
	case EItemType::Consume:
		{
			Apractice_umgCharacter* practice_umgCharacter = Cast<Apractice_umgCharacter>(GetPawn());
			if (IsValid(practice_umgCharacter))
			{
				float HealthRatio = practice_umgCharacter->Heal(ItemTable->ItemFloat1);

				UHealthBar_WS* HealthBar_WS = WidgetMainContainer->HealthBar;
				if (IsValid(HealthBar_WS))
				{
					HealthBar_WS->SetHPRate(HealthRatio);
				}

				TargetInventoryItem->StackCount--;
				if (TargetInventoryItem->StackCount <= 0)
				{
					InventorySlot->CleanUp();
					InventoryItems[TargetItemIndex] = nullptr;
				}
				else
				{
					InventorySlot->UpdateSlot();
				}					
			}
		}
		break;

	case EItemType::Equip:
		{
			Apractice_umgCharacter* practice_umgCharacter = Cast<Apractice_umgCharacter>(GetPawn());
			if (!IsValid(practice_umgCharacter))
				break;

			if (true == TargetInventoryItem->Equipped)
			{
				TargetInventoryItem->Equipped = false;
				InventorySlot->UpdateSlot();
				practice_umgCharacter->ClearWeapon(TargetInventoryItem);
			}
			else
			{
				UInventoryItem* EquipedInventoryItem = nullptr;
				UInventorySlot_WS* EquipedInventorySlot = nullptr;;

				GetEquipedInventoryItem(TargetInventoryItem->GetItemEquipSlot(), EquipedInventoryItem, EquipedInventorySlot);
				if (IsValid(EquipedInventoryItem))
					EquipedInventoryItem->Equipped = false;

				if (IsValid(EquipedInventorySlot))
					EquipedInventorySlot->UpdateSlot();

				TargetInventoryItem->Equipped = true;
				InventorySlot->UpdateSlot();
				practice_umgCharacter->EquipWeapon(TargetInventoryItem);
			}
		}
		break;
	}		
}

void Apractice_umgPlayerController::OnclieckedContextThrow(UInventoryItem* InventoryItem, UInventorySlot_WS* InventorySlot)
{
	if (!IsValid(InventorySlot) || !IsValid(InventorySlot->InventoryItemRef))
		return;

	UInventoryItem* TargetInventoryItem = InventorySlot->InventoryItemRef;
	const FItemTableRow* ItemTable = TargetInventoryItem->GetItemTableRow();

	int TargetItemIndex = InventoryItems.Find(TargetInventoryItem);
	if (INDEX_NONE == TargetItemIndex)
		return;

	if (TargetInventoryItem->Equipped)
	{
		Apractice_umgCharacter* practice_umgCharacter = Cast<Apractice_umgCharacter>(GetPawn());
		if (IsValid(practice_umgCharacter))
		{
			practice_umgCharacter->ClearWeapon(TargetInventoryItem);
			InventoryItems[TargetItemIndex] = nullptr;
			InventorySlot->CleanUp();
			InventorySlot->UpdateSlot();
		}
	}
	else
	{
		InventoryItems[TargetItemIndex] = nullptr;
		InventorySlot->CleanUp();
		InventorySlot->UpdateSlot();
	}

	Apractice_umgCharacter* practice_umgCharacter = Cast<Apractice_umgCharacter>(GetPawn());
	if (IsValid(practice_umgCharacter))
	{
		FTransform SpawnTransform;
		FVector RandomAmount = UKismetMathLibrary::RandomUnitVector();
		RandomAmount *= UKismetMathLibrary::RandomFloatInRange(100.0f, 250.0f);
		RandomAmount.Z = 0.0f;

		SpawnTransform.SetLocation(practice_umgCharacter->GetFootLocation() + RandomAmount);
		SpawnTransform.SetRotation(FRotator(0.0f,UKismetMathLibrary::RandomFloatInRange(0.0f, 360.0f), 0.0f).Quaternion());

		AItem* ThrowedItem = GetWorld()->SpawnActor<AItem>(AItem::StaticClass(), SpawnTransform);
		ThrowedItem->SetItemInfo(TargetInventoryItem->ItemClassID, TargetInventoryItem->StackCount);
	}
}

void Apractice_umgPlayerController::GetEquipedInventoryItem(EItemEquipSlot ItemEquipSlot, UInventoryItem* &InventoryItem, UInventorySlot_WS* &InventorySlot)
{
	InventoryItem = nullptr;
	InventorySlot = nullptr;

	for (int i=0 ; i < 8 ; i++)
	{
		UInventoryItem* CheckedItem = InventoryItems[i];
		if (!IsValid(CheckedItem))
			continue;

		if (true == CheckedItem->Equipped && CheckedItem->GetItemEquipSlot() == ItemEquipSlot)
		{
			InventoryItem = CheckedItem;

			UInventory_WS* Inventory = WidgetMainContainer->Inventory;
			if (IsValid(Inventory))
			{
				InventorySlot = Inventory->GetSlot(i);
			}

			break;
		}
	}
}

int Apractice_umgPlayerController::GetEmptyInvenItem(UInventoryItem*& FoundSlot)
{
	for (int i=0 ; i<MaxInventorySlotCount ; i++)
	{
		if (nullptr == InventoryItems[i])
		{
			UInventoryItem* NewInventoryItem = NewObject<UInventoryItem>(this, UInventoryItem::StaticClass());
			InventoryItems[i] = NewInventoryItem;
			FoundSlot = NewInventoryItem;

			return i;
		}
	}

	return INDEX_NONE;
}

int Apractice_umgPlayerController::FindInventoryItemByItemID(int ItemClassID, UInventoryItem*& FoundSlot/* = nullptr*/)
{
	for (int i = 0; i<MaxInventorySlotCount; i++)
	{
		UInventoryItem* CheckInventoryItem = InventoryItems[i];

		if (nullptr == CheckInventoryItem || !IsValid(CheckInventoryItem))
			continue;

		if (CheckInventoryItem->ItemClassID == ItemClassID)
		{
			FoundSlot = CheckInventoryItem;
			return i;
		}
	}

	return INDEX_NONE;
}