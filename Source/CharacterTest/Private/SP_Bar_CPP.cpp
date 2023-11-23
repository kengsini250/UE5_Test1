// Fill out your copyright notice in the Description page of Project Settings.


#include "SP_Bar_CPP.h"

#include "CharacterTest/MyGameModeBase.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

bool USP_Bar_CPP::Initialize()
{
	Super::Initialize();
	
	//绑定委托
	// auto GameModeT = UGameplayStatics::GetGameMode(GetWorld());
	// auto GameMode = Cast<AMyGameModeBase>(GameModeT);
	// if(GameMode)
	// {
	// 	// UE_LOG(LogTemp,Warning,TEXT("Init Delegate"));
	// 	GameMode->SetSPDelegate.BindUObject(this,&USP_Bar_CPP::SetSP);
	// }
	return true;
}

void USP_Bar_CPP::SetSP(float val)
{
	// UE_LOG(LogTemp,Warning,TEXT("Set Percent"));
	SP->SetPercent(val);
	if(val<0.3)
	{
		SP->SetFillColorAndOpacity(FColor::Yellow);	
	}
	else
	{
		SP->SetFillColorAndOpacity(FColor::Green);	
	}
}
