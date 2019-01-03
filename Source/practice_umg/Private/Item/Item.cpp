// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Item/ItemTableDef.h"
#include "practice_umg_singleton.h"
#include "practice_umg_GameInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TableManager.h"
#include "UMG/ItemName_WS.h"
#include "practice_umgPlayerController.h"
#include "practice_umgCharacter.h"

// Sets default values
AItem::AItem()
{
	ItemSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSphere"));
	ItemSphere->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ItemSphere->SetSphereRadius(150.0f);
	ItemSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	ItemSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	SetRootComponent(ItemSphere);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	ItemMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
	ItemMesh->SetRelativeScale3D(FVector(1.5f));
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);

	ItemClassID = 0;



 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	ItemNameWidget = CreateWidget<UItemName_WS>(UGameplayStatics::GetPlayerController(GetWorld(), 0), UItemName_WS::GetWidgetClassStatic());
	ItemNameWidget->OnItemNameButtonClicked.AddDynamic(this, &AItem::OnItemWidgetButtonClicked);

	SetItemInfo(ItemClassID, StackCount);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	Apractice_umgPlayerController* practice_umgPlayerController = Cast<Apractice_umgPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (IsValid(ItemNameWidget) && ItemNameWidget->GetIsVisible()/*->GetVisibility() == ESlateVisibility::Visible*/)
	{
		FVector2D ScreenPos;
		if (practice_umgPlayerController->ProjectWorldLocationToScreen(GetActorLocation(), ScreenPos))
		{
			ItemNameWidget->SetPositionInViewport(ScreenPos);
		}
	}
}

#if WITH_EDITOR
void AItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AItem, ItemClassID))
	{
		SetItemInfo(ItemClassID, StackCount);
	}
}
#endif

bool AItem::SetItemInfo(int DataTableID, int NewStackCount)
{
	const FItemTableRow* ItemTable = nullptr;

	UTableManager* TableManager = Upractice_umg_GameInstance::GetTableManager();
	if (!IsValid(TableManager))
		return false;

	if (0 == DataTableID)
		ItemTable = nullptr;
	else
		ItemTable = TableManager->GetTable<FItemTableRow>(DataTableID);
	
	if (nullptr == ItemTable)
		return false;

	ItemClassID = ItemTable->ItemClassID;
	ItemMesh->SetStaticMesh(Cast<UStaticMesh>(ItemTable->ItemModel.TryLoad()));

	ItemClassID = DataTableID;
	StackCount = NewStackCount;

	if (IsValid(ItemNameWidget))
	{
		FString ItemName;
		ItemName = ItemTable->ItemName;
		if (StackCount > 1)
		{
			ItemName += TEXT("(");
			ItemName += FString::FromInt(StackCount);
			ItemName += TEXT(")");
		}

		ItemNameWidget->SetItemName(ItemName);
		Apractice_umgPlayerController* practice_umgPlayerController = Cast<Apractice_umgPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		ItemNameWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
	}

	return true;
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(FName(TEXT("ItemPickingBoundary"))) == false)
		return;

	if (Cast<Apractice_umgCharacter>(OtherActor))
	{
		if (IsValid(ItemNameWidget))
		{
			if (!ItemNameWidget->GetIsVisible())
			{
				ItemNameWidget->AddToViewport();
			}
			ItemNameWidget->Showing();
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName(TEXT("ItemPickingBoundary"))) == false)
		return;

	if (Cast<Apractice_umgCharacter>(OtherActor))
	{
		if (IsValid(ItemNameWidget))
		{
			ItemNameWidget->Hiding();
		}
	}
}

void AItem::OnItemWidgetButtonClicked()
{
	Apractice_umgPlayerController* practice_umgPlayerController = Cast<Apractice_umgPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(practice_umgPlayerController))
	{
		if (practice_umgPlayerController->TryPickingItem(ItemClassID, StackCount) == true)
		{
			Destroy();
		}
	}
}