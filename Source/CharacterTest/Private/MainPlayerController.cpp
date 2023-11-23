// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Enemy_HPBar.h"
#include "SP_Bar_CPP.h"
#include "Blueprint/UserWidget.h"


void AMainPlayerController::setSP(float val)
{
	auto SP = HUDOverlay->GetWidgetFromName("SP_Bar");
	if(SP)
	{
		auto MySp = Cast<USP_Bar_CPP>(SP);
		MySp->SetSP(val);
	}
}

void AMainPlayerController::setEnemyHP(float val)
{
	auto temp = Enemy_HPBar_Overlay->GetWidgetFromName("Enemy_HPBar");
	if(temp)
	{
		auto bar = Cast<UEnemy_HPBar>(temp);
		bar->SetEnemyHP(val);
	}
}

void AMainPlayerController::DisplayEnemyHPBar()
{
	if(Enemy_HPBar_Overlay)
	{
		bEnemyHPBar = true;
		Enemy_HPBar_Overlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::HiddenEnemyHPBar()
{
	if(Enemy_HPBar_Overlay)
	{
		bEnemyHPBar = false;
		Enemy_HPBar_Overlay->SetVisibility(ESlateVisibility::Hidden);
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
		Enemy_HPBar_Overlay = CreateWidget<UUserWidget>(this,Enemy_HPBarAsset);
		Enemy_HPBar_Overlay->AddToViewport();
		Enemy_HPBar_Overlay->SetVisibility(ESlateVisibility::Hidden);
		
		//对齐
		FVector2D dd(0,0);
		Enemy_HPBar_Overlay->SetAlignmentInViewport(dd);
	}

}

void AMainPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(Enemy_HPBar_Overlay)
	{
		//2D坐标
		FVector2D PositionInViewport;
		//敌人的3D坐标转换成2D坐标
		ProjectWorldLocationToScreen(EnemyLocation,PositionInViewport);
		//血条UI大小
		const FVector2D UISize = FVector2D(200.f,30.f);
		//位置映射
		Enemy_HPBar_Overlay->SetPositionInViewport(PositionInViewport);
		//大小映射
		Enemy_HPBar_Overlay->SetDesiredSizeInViewport(UISize);
	}
}
