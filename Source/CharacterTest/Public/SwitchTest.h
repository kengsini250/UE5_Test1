// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwitchTest.generated.h"

UCLASS()
class CHARACTERTEST_API ASwitchTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitchTest();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=SwitchTest)
	class UBoxComponent *Box;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=SwitchTest)
	class UStaticMeshComponent *Switch;

	UPROPERTY(BlueprintReadWrite,Category=SwitchTest)
	FVector InitSwitchLocation;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=SwitchTest)
	class UStaticMeshComponent *Door;

	UPROPERTY(BlueprintReadWrite,Category=SwitchTest)
	FVector InitDoorLoaction;

	//计时器
	FTimerHandle SwitchTimer;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=SwitchTest)
	float DoorDelayTime = 2;

	bool bCharacterOnSwitch = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult & SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

	void CloseDoor();

	//BlueprintImplementableEvent 蓝图中调用，C++中不需要实现
	//程序不方便实现的时候，可以在蓝图中实现
	UFUNCTION(BlueprintImplementableEvent,Category=SwitchTest)
	void OpenTheDoor();

	UFUNCTION(BlueprintImplementableEvent,Category=SwitchTest)
	void CloseTheDoor();

	UFUNCTION(BlueprintImplementableEvent,Category=SwitchTest)
	void SwitchON();

	UFUNCTION(BlueprintImplementableEvent,Category=SwitchTest)
	void SwitchOFF();

	UFUNCTION(BlueprintCallable,Category=SwitchTest)
	void UpdateDoorLocation(float z);

	UFUNCTION(BlueprintCallable,Category=SwitchTest)
	void UpdateSwitchLocation(float z);
};
