// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "SP_Bar_CPP.h"
#include "Blueprint/UserWidget.h"


// void AMainPlayerController::setSP(float val)
// {
// 	auto SP = HUDOverlay->GetWidgetFromName("SP_Bar");
// 	if(SP)
// 	{
// 		auto MySp = Cast<USP_Bar_CPP>(SP);
// 		MySp->SetSP(val);
// 	}
// }

void AMainPlayerController::DisplayEnemyHPBar()
{
	if(Enemy_HPBar)
	{
		bEnemyHPBar = true;
		Enemy_HPBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::HiddenEnemyHPBar()
{
	if(Enemy_HPBar)
	{
		bEnemyHPBar = false;
		Enemy_HPBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();


	if(HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this,HUDOverlayAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if(Enemy_HPBarAsset)
	{
		Enemy_HPBar = CreateWidget<UUserWidget>(this,Enemy_HPBarAsset);
		Enemy_HPBar->AddToViewport();
		Enemy_HPBar->SetVisibility(ESlateVisibility::Hidden);
	}
}
