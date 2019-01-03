// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item/ItemTableDef.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "practice_umg_singleton.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_UMG_API Upractice_umg_singleton : public UObject
{
	GENERATED_BODY()
	
	
	
public:
	static Upractice_umg_singleton* Get();

/* Game instance와 TableManager를 이용한 방식으로 테이블 로딩방식 변경
	template<typename T>
	const T* GetTable(int TableKey)
	{
		return GetTable<T>(*FString::Printf(TEXT("%d"), TableKey));
	}

	template<typename T>
	const T* GetTable(FName TableKey)
	{
		UDataTable* TargetTableAsset = nullptr;
		FName TableAssetKey = T::GetTableAssetPath();

		if (DataTableMap.Find(TableAssetKey) == false || DataTableMap[TableAssetKey] == nullptr)
		{
			UDataTable* TableAsset = nullptr;
			FSoftObjectPath TableAssetLoader(*TableAssetKey.ToString());
			TableAsset = Cast<UDataTable>(TableAssetLoader.TryLoad());
			if (!IsValid(TableAsset))
			{
				return nullptr;
			}

			DataTableMap.Add(TableAssetKey, TableAsset);
			TargetTableAsset = TableAsset;
		}
		else
		{
			TargetTableAsset = DataTableMap[TableAssetKey];
		}

		static const FString GetTableCS(TEXT("Upractice_umg_singleton::GetTable"));
		T* RetTable = TargetTableAsset->FindRow<T>(
			TableKey,
			GetTableCS);

		return RetTable;
	}

public:
	UPROPERTY()
	TMap<FName, UDataTable*> DataTableMap;
//*/
};
