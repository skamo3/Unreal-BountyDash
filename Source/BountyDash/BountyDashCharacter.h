// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PowerUpObject.h"
#include "BountyDashCharacter.generated.h"

UCLASS()
class BOUNTYDASH_API ABountyDashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABountyDashCharacter();

	UPROPERTY(EditAnywhere, Category = Logic)
	TArray<class ATargetPoint*> TargetArray;

	UPROPERTY(EditAnywhere, Category = Logic)
	float CharSpeed;

	UPROPERTY(EditAnywhere, Category = Sound)
	UAudioComponent* hitObstacleSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	UAudioComponent* dingSound;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadOnly)
	int32 Score;

	void MoveRight();
	void MoveLeft();

	UFUNCTION()
	void MyOnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void MyOnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = PowerUps)
	float SmashTime;

	UPROPERTY(EditAnywhere, Category = PowerUps)
	float MagnetTime;

	UPROPERTY(EditAnywhere, Category = PowerUps)
	float MagnetReach;

	UFUNCTION()
	void StopSmash();

	UFUNCTION()
	void StopMagnet();

	void CoinMagnet();

	UFUNCTION()
	void Reset();

	float Killpoint;

private:
	short CurrentLocation;
	FVector DesiredLocation;
	bool bBeingPushed;
	bool CanSmash;
	bool CanMagnet;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ScoreUp();
	void PowerUp(EPowerUp Type);
	int GetScore();
};
