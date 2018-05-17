// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetBoxExtent(FVector(32, 10, 10));

	Bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));
	Bullet->SetupAttachment(Box);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Bullet(TEXT("StaticMesh'/Game/Meshes/Talon.Talon'"));
	if (SM_Bullet.Succeeded())
	{
		Bullet->SetStaticMesh(SM_Bullet.Object);
	}
	Bullet->SetRelativeLocation(FVector(-30, 0, 0));
	Bullet->SetRelativeRotation(FRotator(-90, 0, 0));
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 1000.0f;
	Movement->MaxSpeed = 1000.0f;
	Movement->ProjectileGravityScale = 0.4f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Explosion(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (P_Explosion.Succeeded())
	{
		Explosion = P_Explosion.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> S_Explosion(TEXT("SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'"));
	if (S_Explosion.Succeeded())
	{
		ExplosionSound = S_Explosion.Object;
	}
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABullet::Timer, 3.0f, false);
	//GetWorldTimerManager().ClearTimer(TimerHandle);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ABullet::BeginOverlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::Timer()
{
	//UE_LOG(LogClass, Warning, TEXT("Destroy %s"), *this->GetName());
	Destroy();
}

void ABullet::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult & SweepResult) {

	if (Cast<APawn>(OtherActor) == nullptr)
	{
		//UE_LOG(LogClass, Warning, TEXT("Overlap! %s"), *OtherActor->GetName());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion,
			GetActorLocation(), GetActorRotation(), true);

		UGameplayStatics::SpawnSoundAtLocation(this,
			ExplosionSound, GetActorLocation());

		Destroy();
	}
}