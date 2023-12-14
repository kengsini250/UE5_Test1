// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Property_CPP.h"

#include "Components/TextBlock.h"

bool UCharacter_Property_CPP::Initialize()
{
	Super::Initialize();

	return true;
}

void UCharacter_Property_CPP::changeValue(FString s)
{
	Exp_Bar->SetText(FText::FromString(s));
}
