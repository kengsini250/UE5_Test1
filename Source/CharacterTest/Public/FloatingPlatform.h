// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class CHARACTERTEST_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

	FTimerHandle InterpTimer;

	//地板
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Platform")
	class UStaticMeshComponent *Mesh;

	//移动速度
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Platform")
	float Speed = 4;

	//停止时间
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Platform")
	float InterpTime = 1;

	//反向运动
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Platform")
	bool bInterping = false;

	//起始位置
	UPROPERTY(EditAnywhere)
	FVector StartPoint = FVector(0);

	//结束位置
	UPROPERTY(EditAnywhere,meta=(MakeEditWidget="true"))
	FVector EndPoint = FVector(0);

	//距离
	float Distance = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleInterping();
};
