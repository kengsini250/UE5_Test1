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
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Item | Name")
	FString WeaponName;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="SleletalMesh")
	class USkeletalMeshComponent* skeletalMesh;

	//武器与角色碰撞->攻击
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Item | Hit")
	class UCapsuleComponent* HitCapsule;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item | Hit")
	float Damage = 30.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item | Sound")
	class USoundCue *SwingSound;
	
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

	UFUNCTION(BlueprintCallable)
	void CollisionON();
	
	UFUNCTION(BlueprintCallable)
	void CollisionOFF();

	//伤害机制
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	TSubclassOf<UDamageType> DamageTypeClass;
	//伤害机制触发对象
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	AController *UseWeaponTarget;

	FORCEINLINE void setUseWeaponTarget(AController*c){UseWeaponTarget = c;}
};
