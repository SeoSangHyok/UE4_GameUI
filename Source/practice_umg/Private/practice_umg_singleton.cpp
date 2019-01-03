// Fill out your copyright notice in the Description page of Project Settings.

#include "practice_umg_singleton.h"
#include "Engine/Engine.h"




Upractice_umg_singleton* Upractice_umg_singleton::Get()
{
	Upractice_umg_singleton* SingletonObject = Cast<Upractice_umg_singleton>(GEngine->GameSingleton);
	if (IsValid(SingletonObject))
	{
		return SingletonObject;
	}
	else
	{
		return NewObject<Upractice_umg_singleton>(nullptr, Upractice_umg_singleton::StaticClass());
	}
}