// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_HPBar.h"

#include "CharacterTest/MyGameModeBase.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

bool UEnemy_HPBar::Initialize()
{
	Super::Initialize();
	//绑定委托
	auto GameModeT = UGameplayStatics::GetGameMode(GetWorld());
	auto GameMode = Cast<AMyGameModeBase>(GameModeT);
	if(GameMode)
	{
		// UE_LOG(LogTemp,Warning,TEXT("Init Delegate"));
		GameMode->Enemy_SetHPDelegate.AddUObject(this,&UEnemy_HPBar::SetEnemyHP);
	}
	return true;
}

void UEnemy_HPBar::SetEnemyHP(float val)
{
	ProgressBar_Enemy_HPBar->SetPercent(val);
}
