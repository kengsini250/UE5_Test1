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
	auto Enemy_HPBar = EnemyHUDOverlay->GetWidgetFromName("Enemy_HPBar");
	if(Enemy_HPBar)
	{
		auto temp = Cast<UEnemy_HPBar>(Enemy_HPBar);
		temp->SetEnemyHP(val);
	}
}

void AMainPlayerController::DisplayEnemyHPBar()
{
		bEnemyHPBar = true;
		EnemyHUDOverlay->SetVisibility(ESlateVisibility::Visible);
}

void AMainPlayerController::HiddenEnemyHPBar()
{
		bEnemyHPBar = false;
		EnemyHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
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
		EnemyHUDOverlay = CreateWidget<UUserWidget>(this,Enemy_HPBarAsset);
		EnemyHUDOverlay->AddToViewport();
		EnemyHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
		
		//对齐
		FVector2D dd(0,0);
		EnemyHUDOverlay->SetAlignmentInViewport(dd);
	}

}

void AMainPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	auto Enemy_HPBar = EnemyHUDOverlay->GetWidgetFromName(TEXT("Enemy_HPBar"));
	if(Enemy_HPBar)
	{
		//2D坐标
		FVector2D PositionInViewport;
		//敌人的3D坐标转换成2D坐标
		ProjectWorldLocationToScreen(EnemyLocation,PositionInViewport);
		PositionInViewport.Y -=230;
		PositionInViewport.X -=130;
		//血条UI大小
		const FVector2D UISize = FVector2D(200.f,30.f);
		
		//位置映射
		EnemyHUDOverlay->SetPositionInViewport(PositionInViewport);
		//大小映射
		EnemyHUDOverlay->SetDesiredSizeInViewport(UISize);
	}
}
