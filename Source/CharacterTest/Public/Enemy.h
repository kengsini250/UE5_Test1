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
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveToTarget(class AMainCharacter* self);
	
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
};
