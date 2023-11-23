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
	auto EnemyHPBar = Enemy_HPBar->GetWidgetFromName("Enemy_HPBar");
	if(EnemyHPBar)
	{
		auto MyEnemyHP = Cast<UEnemy_HPBar>(EnemyHPBar);
		MyEnemyHP->SetEnemyHP(val);
	}
}

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
		
		//对齐
		FVector2D dd(0,0);
		Enemy_HPBar->SetAlignmentInViewport(dd);
	}

}

void AMainPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(Enemy_HPBar)
	{
		//2D坐标
		FVector2D PositionInViewport;
		//敌人的3D坐标转换成2D坐标
		ProjectWorldLocationToScreen(EnemyLocation,PositionInViewport);
		//血条UI大小
		const FVector2D UISize = FVector2D(200.f,30.f);
		//位置映射
		Enemy_HPBar->SetPositionInViewport(PositionInViewport);
		//大小映射
		Enemy_HPBar->SetDesiredSizeInViewport(UISize);
	}
}
