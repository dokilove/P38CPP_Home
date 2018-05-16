// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "P38Pawn.generated.h"

UCLASS()
class P38CPP_API AP38Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AP38Pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, category="Roootate")
	void Rotate(UStaticMeshComponent * Mesh);
	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void MoveRight(float Value);
	UFUNCTION()
	void Fire();

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, category="Component")
	class UBoxComponent* Box;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, category = "Component")
	class UStaticMeshComponent* Body;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, category = "Component")
	class UStaticMeshComponent* Left;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, category = "Component")
	class UStaticMeshComponent* Right;


	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, category = "Component")
	class UFloatingPawnMovement* Movement;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	class UArrowComponent* Arrow;

	// 블루프린트 사용하는 방법 클릭해서 선택
	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	class TSubclassOf<class ABullet> BulletBlueprint;

	// 블루프린트 사용하는 방법 경로찾아 생성
	class UClass* Bullet;
};
