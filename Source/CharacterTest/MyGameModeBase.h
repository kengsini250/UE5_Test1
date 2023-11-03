// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

//定义委托
DECLARE_MULTICAST_DELEGATE_OneParam(FSetSPDelegate,float)
DECLARE_MULTICAST_DELEGATE_OneParam(FEnemy_SetHPDelegate,float)

/**
 * 
 */
UCLASS()
class CHARACTERTEST_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	//声明委托
	FSetSPDelegate SetSPDelegate;
	FEnemy_SetHPDelegate Enemy_SetHPDelegate;
};
