// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreateNPC_Test.generated.h"

UCLASS()
class CHARACTERTEST_API ACreateNPC_Test : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreateNPC_Test();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=CreateNPC)
	class UBoxComponent *Base;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=CreateNPC)
	TSubclassOf<class ANPC_Test> NPCs;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure,Category=CreateNPC)
	FVector GetRandomPoint();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category=CreateNPC)
	void CreateNPC(UClass* NewNPC,const FVector& Location);
	
};
