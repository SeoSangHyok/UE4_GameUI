// Fill out your copyright notice in the Description page of Project Settings.

#include "PracticeUMGWidget.h"
#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"



void UPracticeUMGWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UClass* WidgetClass = GetWidgetClass();

	if (nullptr != WidgetClass)
	{
//*
		for (TFieldIterator<UObjectProperty> PropIt(WidgetClass); PropIt; ++PropIt)
		{
			UObjectProperty* Property = *PropIt;
			if (IsValid(Property))
			{
				UObject* TestObj = Property->GetObjectPropertyValue_InContainer(this);
				UWidgetAnimation* WidgetAnimation = Cast<UWidgetAnimation>(TestObj);
				if (IsValid(WidgetAnimation))
				{
					AnimationList.Add(Property->GetFName(), WidgetAnimation);
				}
			}
 		}

//*/

/* 아래처럼도 처리가능
		UProperty* prop = WidgetClass->PropertyLink;
		// Run through all properties of this class to find any widget animations
		while (prop != nullptr)
		{
			// Only interested in object properties
			if (prop->GetClass() == UObjectProperty::StaticClass())
			{
				UObjectProperty* objectProp = Cast<UObjectProperty>(prop);

				// Only want the properties that are widget animations
				if (objectProp->PropertyClass == UWidgetAnimation::StaticClass())
				{
					UObject* object = objectProp->GetObjectPropertyValue_InContainer(this);

					UWidgetAnimation* widgetAnim = Cast<UWidgetAnimation>(object);

					if (widgetAnim != nullptr)
					{
						AnimationList.Add(objectProp->GetFName(), widgetAnim);

						// DO SOMETHING TO STORE OFF THE ANIM PTR HERE!
						// E.g. add to a TArray of some struct that holds info for each anim
					}
				}
			}

			prop = prop->PropertyLinkNext;
		}
//*/
	}
}