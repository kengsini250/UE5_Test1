// Fill out your copyright notice in the Description page of Project Settings.


#include "LV_Bar_Cpp.h"

#include "Components/TextBlock.h"

bool ULV_Bar_Cpp::Initialize()
{
	return Super::Initialize();
}

void ULV_Bar_Cpp::changeLV(int s)
{
	Lv_Bar->SetText(FText::FromString(FString::FromInt(s)));
}
