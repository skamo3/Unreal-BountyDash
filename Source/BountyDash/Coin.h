// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BountyDashObject.h"
#include "Coin.generated.h"

/**
 * 
 */
UCLASS()
class BOUNTYDASH_API ACoin : public ABountyDashObject
{
	GENERATED_BODY()

	ACoin();

	virtual void Tick(float DeltaTime) override;

	virtual void MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;

public:
	bool BeingPulled;
};
