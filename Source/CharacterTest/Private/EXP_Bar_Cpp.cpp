// Fill out your copyright notice in the Description page of Project Settings.


#include "EXP_Bar_Cpp.h"

#include "Components/TextBlock.h"

bool UEXP_Bar_Cpp::Initialize()
{
	return Super::Initialize();
}

void UEXP_Bar_Cpp::changeEXP(int s)
{
	Exp_Bar->SetText(FText::FromString(FString::FromInt(s)));
}
