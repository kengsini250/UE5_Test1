// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SP_Bar_CPP.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERTEST_API USP_Bar_CPP : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* SP;

	UFUNCTION()
	void SetSP(float val);
};
