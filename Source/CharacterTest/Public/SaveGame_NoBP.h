// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGame_NoBP.generated.h"

USTRUCT(Blueprintable)
struct FCharacterState
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere,Category = "SaveData")
	float HP;
	UPROPERTY(VisibleAnywhere,Category = "SaveData")
	float HP_Max;
	UPROPERTY(VisibleAnywhere,Category = "SaveData")
	float SP;
	UPROPERTY(VisibleAnywhere,Category = "SaveData")
	float SP_Max;
	UPROPERTY(VisibleAnywhere,Category = "SaveData")
	FVector Location;
	UPROPERTY(VisibleAnywhere,Category = "SaveData")
	FRotator Rotator;
};
/**
 * 
 */
UCLASS()
class CHARACTERTEST_API USaveGame_NoBP : public USaveGame
{
	GENERATED_BODY()

public:
	USaveGame_NoBP();
	
	UPROPERTY(VisibleAnywhere,Category = Basic)
	FString SaveName;
	
	UPROPERTY(VisibleAnywhere,Category = Basic)
	uint32 SaveID;
	
	UPROPERTY(VisibleAnywhere,Category = Basic)
	FCharacterState characterState;
};
