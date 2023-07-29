// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERTEST_API AWeapon : public AItem
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	AWeapon();
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SleletalMesh")
	class USkeletalMeshComponent* skeletalMesh;

	//武器与角色碰撞->攻击
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Item | Hit")
	class UCapsuleComponent* HitCapsule;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item | Hit")
	float Damage = 30.0f;
	
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult)override;
 
	virtual void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex)override;

	void Equip(class AMainCharacter* mainC);

	UFUNCTION()
	void HitCapsuleOnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult);
 
	UFUNCTION()
	void HitCapsuleOnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);
};
