// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EMS_Idle UMETA(DeplayName="Idle"),
	EMS_MoveToTarget UMETA(DeplayName="MoveToTarget"),
	EMS_Attacking UMETA(DeplayName="Attacking"),
	EMS_Dead UMETA(DeplayName="Dead"),
	EMS_MAX
};


UCLASS()
class CHARACTERTEST_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Movement")
	EEnemyMovementStatus EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;
	FORCEINLINE void SetMovement(EEnemyMovementStatus status)
	{
		EnemyMovementStatus = status;
	}
	FORCEINLINE EEnemyMovementStatus GetMovementStatus()
	{
		return EnemyMovementStatus;
	}

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AI")
	class AAIController* AIController;

	//索敌范围-box
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AI")
	class UBoxComponent* Box;
	//命中判定-胶囊体
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AI")
	class UCapsuleComponent* HitCapsule;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Running")
	float RunningSpeed = 650;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	float HP_Max = 100.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	float HP = 100.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	float Damage = 10.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item | Sound")
	class USoundCue *HitSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item | Sound")
	USoundCue *SwingSound;

	//武器与角色碰撞->攻击
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Item | Hit")
	UCapsuleComponent* WeaponCollision;

	//流血粒子特效
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	class UParticleSystem* HitParticles;
	
	void BloodParticles(const FVector& pos);

	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	float AttackTime = 3;

	//伤害机制
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	TSubclassOf<UDamageType> DamageTypeClass;

	FTimerHandle DeathDestroyTimer;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	float DeathDestroyDelay = 2;

	void EnemyDestroy();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMainCharacter* s);
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AI")
	bool bOverlappingHitCapsule = false;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AI")
	AMainCharacter* HitTarget;

	void Die();
	
	virtual float TakeDamage(float _Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION()
	void BoxOnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult & SweepResult);

	UFUNCTION()
	void BoxOnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

	
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

	UFUNCTION()
	void WeaponOnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult);
 
	UFUNCTION()
	void WeaponOnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void AttackCollisionStart();
	
	UFUNCTION(BlueprintCallable)
	void AttackCollisionEnd();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Anims")
	class UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere)
	bool bAttacking = false;
	
	void Attack();
	
	UFUNCTION(BlueprintCallable)
	void EnemyAttackEnd();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	bool Alive();
};
