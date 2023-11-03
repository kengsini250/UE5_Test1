// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERTEST_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widgets")
	UUserWidget* HUDOverlay;

	//也可以
	// void setSP(float val);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widgets")
	TSubclassOf<UUserWidget> Enemy_HPBarAsset;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widgets")
	UUserWidget* Enemy_HPBar;

	bool bEnemyHPBar = false;
	void DisplayEnemyHPBar();
	void HiddenEnemyHPBar();
protected:
	virtual void BeginPlay() override;

private:
};
