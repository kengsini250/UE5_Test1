// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EXP_Bar_Cpp.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERTEST_API UEXP_Bar_Cpp : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* Exp_Bar;

	void changeEXP(int s);
};
