// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal
	UMETA(DisplayName="Normal"),
	
	EMS_Sprinting
	UMETA(DisplayName="Sprinting"),

	EMS_Dead
	UMETA(DisplayName="Dead"),

	EMS_MAX
	UMETA(DisplayName="MAX")
};

UENUM(BlueprintType)
enum class ESpStatus : uint8
{
	ESS_Normal
	UMETA(DisplayName="Normal"),
	
	ESS_BelowMinimum
	UMETA(DisplayName="BelowMinimum"),//低于一定百分比
	
	ESS_Exhausted
	UMETA(DisplayName="Exhausted"),//体力耗尽
	
	ESS_ExhaustedRecovering
	UMETA(DisplayName="ExhaustedRecovering"),//体力恢复
	
	ESS_MAX
	UMETA(DisplayName="MAX")
};

UCLASS()
class CHARACTERTEST_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();
	
	// class AGameModeBase* GameModeT; 
	// class AMyGameModeBase* GameMode;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Controller")
	class AMainPlayerController* mainController;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Items")
	class AWeapon* Weapon;

	//捡武器-》与武器重叠
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Items")
	class AItem* OverlappingItem;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Enums")
	EMovementStatus MovementStatus = EMovementStatus::EMS_Normal;
	void SetMovementStatus(EMovementStatus status);

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Running")
	float RunningSpeed = 650;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Running")
	float SprintingSpeed = 950;

	bool bShiftKeyDown = false;
	void ShiftKeyDown();
	void ShiftKeyUp();

	//扔武器 G
	bool bGiveWeapon = false;
	void G_Up();
	void G_Down();

	bool b_C_KeyDown = false;
	void C_Down();
	void C_Up();

	UPROPERTY(VisibleAnywhere)
	bool bLMBDown = false;
	UPROPERTY(VisibleAnywhere)
	bool bAttacking = false;
	void LMBUp();
	void LMBDown();
	void Attack();

	void Die();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	virtual void Jump() override;
	
	//蓝图调用
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Anims")
	class UAnimMontage* AttackMontage;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Enums")
	ESpStatus SpStatus = ESpStatus::ESS_Normal;
	void SetSpStatus(ESpStatus status);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivate="ture"))
	class USpringArmComponent *CameraArm;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivate="ture"))
	class UCameraComponent *FollowCamera;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera")
	float BaseTurnRate = 100;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera")
	float BaseLookUpRate = 100;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Player State")
	float HP_Max = 100;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Player State")
	float SP_Max = 100;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Player State")
	float HP = 70;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Player State")
	float SP = 100;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Player State")
	int32 Coins = 0;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Player State")
	float SpRate = 10;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Player State")
	float SpMinStatus = 30;

	//流血粒子特效
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Particles")
	class UParticleSystem* HitParticles;
	
	//被击音效
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Item | Sound")
	class USoundCue *HitSound;

	float InterpSpeed = 15;
	bool bInterpToEnemy = false;
	void SetInterpToEnemy(bool Interp);
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,category="Player State")
	class AEnemy*InterpTarget;
	FORCEINLINE void SetInterpTarget(AEnemy* e){InterpTarget = e;}
	FRotator GetLookAtRotatorYaw(FVector target);

	//敌人坐标
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere,Category="Target")
	FVector EnemyTargetLocation;
	//判断是否有重叠，用来显示/隐藏血条UI
	bool bHasCurrEnemy;
	FORCEINLINE void setHasCurrEnemy(bool curr){bHasCurrEnemy = curr;}
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void MoveForward(float value);
	void MoveRight(float value);
	void TurnAtRate(float value);
	void LookUpAtRate(float value);
	void LookUp(float value);
	void Turn(float value);
	

	UFUNCTION(BlueprintCallable)
	float getCurrHP();
	
	UFUNCTION(BlueprintCallable)
	float getCurrHP_Per();
	
	UFUNCTION(BlueprintCallable)
	void setHP(float value);

	FORCEINLINE void setWeapon(AWeapon* w)
	{
		Weapon = w;
	}
	
	FORCEINLINE AWeapon* getWeapon()
	{
		return Weapon;
	}

	FORCEINLINE void setOverlappingItem(AItem* w)
	{
		OverlappingItem = w;
	}

	void switchLevel(FName name);

	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPos);
	void LoadGameNoSwitch();

	UPROPERTY(EditAnywhere,Category = "Save")
	TSubclassOf<class ASaveWeapon_BP> SaveWeaponMap;

	bool bESC = false;
	void EscUp();
	void EscDown();
};
