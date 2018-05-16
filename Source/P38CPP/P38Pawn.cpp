// Fill out your copyright notice in the Description page of Project Settings.

#include "P38Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Bullet.h"

// Sets default values
AP38Pawn::AP38Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("StaticMesh'/Game/Meshes/Chainsaw.Chainsaw'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}
	Body->SetRelativeLocation(FVector(-50, 0, 0));
	Body->SetRelativeRotation(FRotator(0, -90, 90));

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);
	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Prop(TEXT("StaticMesh'/Game/Meshes/DeadlyAxe.DeadlyAxe'"));
	if (SM_Prop.Succeeded())
	{
		Left->SetStaticMesh(SM_Prop.Object);
		Right->SetStaticMesh(SM_Prop.Object);
	}
	Left->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	Left->SetRelativeLocation(FVector(30, 0, 0));
	Right->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));
	Right->SetRelativeLocation(FVector(-30, 0, 0));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = 400.0f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	Arrow->SetRelativeLocation(FVector(40, 0, -20));

	static ConstructorHelpers::FObjectFinder<UBlueprint> BP_Bullet(TEXT("Blueprint'/Game/Blueprints/BP_BulletCPP.BP_BulletCPP'"));
	if (BP_Bullet.Succeeded())
	{
		Bullet = Cast<UClass>(BP_Bullet.Object->GeneratedClass);
	}
}

// Called when the game starts or when spawned
void AP38Pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AP38Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotate(Left);
	Rotate(Right);

	AddMovementInput(GetActorForwardVector(), 1.0f);
}

// Called to bind functionality to input
void AP38Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AP38Pawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AP38Pawn::MoveRight);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this,
		&AP38Pawn::Fire);
}

void AP38Pawn::Rotate(UStaticMeshComponent* Mesh)
{
	float DeltaSecond = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	Mesh->AddRelativeRotation(FRotator(0, 720 * DeltaSecond, 0));
}

void AP38Pawn::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		//UE_LOG(LogClass, Warning, TEXT("MoveForward %f"), Value);
		float DeltaSecond = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		AddActorLocalRotation(FRotator(90 * DeltaSecond * -Value, 0, 0));
	}
}

void AP38Pawn::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		//UE_LOG(LogClass, Warning, TEXT("MoveRight %f"), Value);
		float DeltaSecond = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		AddActorLocalRotation(FRotator(0, 0, 90 * DeltaSecond * Value));
	}
}

void AP38Pawn::Fire()
{
	//GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(),
	//	Arrow->GetComponentLocation(), Arrow->GetComponentRotation());

	//UE_LOG(LogClass, Warning, TEXT("Fire!"));

	// 블루프린트 사용하는 방법 클릭해서 선택
	//GetWorld()->SpawnActor<ABullet>(BulletBlueprint,
	//	Arrow->GetComponentLocation(), Arrow->GetComponentRotation());

	// 블루프린트 사용하는 방법 경로찾아 생성
	GetWorld()->SpawnActor<ABullet>(Bullet,
		Arrow->GetComponentLocation(), Arrow->GetComponentRotation());
}
