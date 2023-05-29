// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionMesh = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionMesh"));
	RootComponent = CollisionMesh;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	ParticlesComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticlesComponent"));
	ParticlesComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this,&AItem::OnOverlapBegin);
	CollisionMesh->OnComponentEndOverlap.AddDynamic(this,&AItem::OnOverlapEnd);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bRotate)
	{
		FRotator R = GetActorRotation();
		R.Yaw += DeltaTime*RotateRate;
		SetActorRotation(R);
	}
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OverlapParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			OverlapParticles,
			GetActorLocation(),
			FRotator(0),
			true);
	}
	if(OverlapSound)
	{
		UGameplayStatics::PlaySound2D(this,OverlapSound);
	}
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}