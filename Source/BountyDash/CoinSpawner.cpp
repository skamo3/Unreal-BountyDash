// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinSpawner.h"
#include "BountyDash.h"
#include "Engine/TargetPoint.h"
#include "BountyDashPowerUp.h"
#include "EngineUtils.h"
#include "Coin.h"
#include "Floor.h"

// Sets default values
ACoinSpawner::ACoinSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	this->MaxSetCoins = 5;
	this->MinSetCoins = 3;
	this->CoinSetTimeInterval = 4.0f;
	this->CoinTimeInterval = 0.5f;
	this->MovementTimeInterval = 1.0f;

}

// Called when the game starts or when spawned
void ACoinSpawner::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		this->SPawnTransforms.Add(*TargetIter);
	}
	for (TActorIterator<AFloor> FloorIter(GetWorld()); FloorIter; ++FloorIter)
	{
		if (FloorIter->GetWorld() == GetWorld())
		{
			this->KillPoint = FloorIter->GetKillPoint();
			this->SpawnPoint = FloorIter->GetSpawnPoint();
		}
	}

	FTimerManager& worldTimeManager = GetWorld()->GetTimerManager();

	worldTimeManager.SetTimer(this->CoinSetTimerHandle, this, &ACoinSpawner::SpawnCoinSet, this->CoinSetTimeInterval, false);
	worldTimeManager.SetTimer(this->SpawnMoveTimerHandle, this, &ACoinSpawner::MoveSpawner, this->MovementTimeInterval, true);

	
}


void ACoinSpawner::SpawnCoinSet()
{
	this->NumCoinsToSpawn = FMath::RandRange(this->MinSetCoins, this->MaxSetCoins);

	FTimerManager& worldTimeManager = GetWorld()->GetTimerManager();

	worldTimeManager.ClearTimer(this->CoinSetTimerHandle);

	worldTimeManager.SetTimer(this->CoinTimerHandle, this, &ACoinSpawner::SpawnCoin, this->CoinTimeInterval, true);
}

void ACoinSpawner::MoveSpawner()
{
	this->TargetLoc = FMath::Rand() % this->SPawnTransforms.Num();
}

void ACoinSpawner::SpawnCoin()
{
	if (FMath::Rand() % 100 < PowerUpChance)
	{
		SpawnPowerUp();
		NumCoinsToSpawn--;
	}
	else
	{
		FActorSpawnParameters spawnParams;

		FTransform coinTransform = SPawnTransforms[this->TargetLoc]->GetTransform();

		coinTransform.SetLocation(FVector(this->SpawnPoint, coinTransform.GetLocation().Y, coinTransform.GetLocation().Z));

		ACoin* spawnedCoin = GetWorld()->SpawnActor<ACoin>(this->CoinObject, coinTransform, spawnParams);

		if (spawnedCoin)
		{
			spawnedCoin->SetKillPoint(KillPoint);
			USphereComponent* coinSphere = Cast<USphereComponent>(spawnedCoin->GetComponentByClass(USphereComponent::StaticClass()));

			if (coinSphere)
			{
				float offset = coinSphere->GetUnscaledSphereRadius();
				spawnedCoin->AddActorLocalOffset(FVector(0.0f, 0.0f, offset));
			}
			this->NumCoinsToSpawn--;
		}
	}
	
	if (this->NumCoinsToSpawn <= 0)
	{
		FTimerManager& worldTimeManager = GetWorld()->GetTimerManager();
		worldTimeManager.SetTimer(CoinSetTimerHandle, this, &ACoinSpawner::SpawnCoinSet, CoinSetTimeInterval, false);
		worldTimeManager.ClearTimer(this->CoinTimerHandle);
	}
}

void ACoinSpawner::SpawnPowerUp()
{
	FActorSpawnParameters SpawnInfo;

	FTransform myTrans = SPawnTransforms[TargetLoc]->GetTransform();
	myTrans.SetLocation(FVector(SpawnPoint, myTrans.GetLocation().Y, myTrans.GetLocation().Z));

	ABountyDashPowerUp* newObs = GetWorld()->SpawnActor<ABountyDashPowerUp>(PowerUpObject, myTrans, SpawnInfo);

	if (newObs)
	{
		newObs->SetKillPoint(KillPoint);
		USphereComponent* powerUpSphere = Cast<USphereComponent>(newObs->GetComponentByClass(USphereComponent::StaticClass()));

		if (powerUpSphere)
		{
			float offset = powerUpSphere->GetUnscaledSphereRadius();
			newObs->AddActorLocalOffset(FVector(0.0f, 0.0f, offset));
		}
	}
}
