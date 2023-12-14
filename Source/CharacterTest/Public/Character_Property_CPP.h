// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character_Property_CPP.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERTEST_API UCharacter_Property_CPP : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Exp_Bar;

	UFUNCTION()
	void changeValue(FString s);
};
