// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewLevel.generated.h"

UCLASS()
class CHARACTERTEST_API ANewLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANewLevel();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="NewMap")
	class UBoxComponent* TransitionVolume;

	class UBillboardComponent* Billboard;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="NewMap")
	FName MapName;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);
};
