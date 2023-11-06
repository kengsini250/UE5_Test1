// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enemy_HPBar.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERTEST_API UEnemy_HPBar : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual bool Initialize() override;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* ProgressBar_Enemy_HPBar;

	UFUNCTION()
	void SetEnemyHP(float val);
};
