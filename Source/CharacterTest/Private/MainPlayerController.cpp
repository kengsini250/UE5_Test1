// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Enemy_HPBar.h"
#include "EXP_Bar_Cpp.h"
#include "LV_Bar_Cpp.h"
#include "MainCharacter.h"
#include "SP_Bar_CPP.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"


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

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	bPauseMenu=true;
	PauseMenu->SetVisibility(ESlateVisibility::Visible);

	FInputModeGameAndUI mode;
	SetInputMode(mode);
	bShowMouseCursor=true;
}

void AMainPlayerController::HiddenPauseMenu_Implementation()
{
	bPauseMenu=false;
	// PauseMenu->SetVisibility(ESlateVisibility::Hidden);//蓝图中设置

	FInputModeGameOnly mode;
	SetInputMode(mode);
	bShowMouseCursor=true;
}

void AMainPlayerController::TogglePauseMenu()
{
	if(bPauseMenu)
	{
		HiddenPauseMenu();
	}else
	{
		DisplayPauseMenu();
	}
}

void AMainPlayerController::DisplayCharactorPropertyHUD()
{
	bCharactorPropertyHUD = true;
	Character_Property->SetVisibility(ESlateVisibility::Visible);
}

void AMainPlayerController::HiddenCharactorPropertyHUD()
{
	bCharactorPropertyHUD = false;
	Character_Property->SetVisibility(ESlateVisibility::Hidden);
}

void AMainPlayerController::changeEXP(int s)
{
	auto currCharacter = Cast<AMainCharacter>(GetCharacter());
	auto EXP = Character_Property->GetWidgetFromName("EXP_Bar");
	if(EXP)
	{
		auto EXP_Bar = Cast<UEXP_Bar_Cpp>(EXP);
		if(EXP_Bar)
		{
			int UP = 50;
			int temp = s - UP;
			if(temp>0)
			{
				EXP_Bar->changeEXP(temp);
				changeLV(++currCharacter->LV);
			}else
			{
				int te=currCharacter->EXP+s;
				EXP_Bar->changeEXP(te);
			}
		}
	}
}

void AMainPlayerController::changeLV(int s)
{
	auto LV = Character_Property->GetWidgetFromName("LV_Bar");
	if(LV)
	{
		auto LV_Bar = Cast<ULV_Bar_Cpp>(LV);
		LV_Bar->changeLV(s);
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
		EnemyHUDOverlay = CreateWidget<UUserWidget>(this,Enemy_HPBarAsset);
		EnemyHUDOverlay->AddToViewport();
		EnemyHUDOverlay->SetVisibility(ESlateVisibility::Hidden);
		
		//对齐
		FVector2D dd(0,0);
		EnemyHUDOverlay->SetAlignmentInViewport(dd);
	}

	if(PauseMenuAsset)
	{
		PauseMenu = CreateWidget<UUserWidget>(this,PauseMenuAsset);
		PauseMenu->AddToViewport();
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
	}

	if(Character_Property_Asset)
	{
		Character_Property = CreateWidget<UUserWidget>(this,Character_Property_Asset);
		Character_Property->AddToViewport();
		Character_Property->SetVisibility(ESlateVisibility::Hidden);
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
