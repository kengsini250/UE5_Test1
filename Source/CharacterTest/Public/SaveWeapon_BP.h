// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveWeapon_BP.generated.h"

UCLASS()
class CHARACTERTEST_API ASaveWeapon_BP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveWeapon_BP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere,Category = "Save")
	TMap<FString,TSubclassOf<class AWeapon>> WeaponMap;
};
