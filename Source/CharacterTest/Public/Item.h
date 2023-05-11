// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class CHARACTERTEST_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Item | CollisionMesh")
	class USphereComponent* CollisionMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "Item | Mesh")
	class UStaticMeshComponent *Mesh;

	//粒子系统
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Particles")
	class UParticleSystemComponent* ParticlesComponent;
	
	//没有component则不需要用CreateDefaultSubobject
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Particles")
	class UParticleSystem* OverlapParticles;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Sound")
	class USoundCue* OverlapSound;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Property")
	bool bRotate = false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Item | Property")
	float RotateRate = 30;
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
 
	UFUNCTION()
	virtual void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);
};

