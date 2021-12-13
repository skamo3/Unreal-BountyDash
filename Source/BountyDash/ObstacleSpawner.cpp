// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleSpawner.h"
#include "BountyDash.h"
#include "BountyDashGameMode.h"
#include "Engine/TargetPoint.h"
#include "EngineUtils.h"
#include "Floor.h"
#include "Obstacle.h"

// Sets default values
AObstacleSpawner::AObstacleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	check(this->Scene);
	RootComponent = this->Scene;

	this->SpawnTimer = 1.5f;
}

// Called when the game starts or when spawned
void AObstacleSpawner::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		this->SpawnTargets.Add(*TargetIter);
	}
	for (TActorIterator<AFloor> FloorIter(GetWorld()); FloorIter; ++FloorIter)
	{
		if (FloorIter->GetWorld() == GetWorld())
		{
			this->KillPoint = FloorIter->GetKillPoint();
			this->SpawnPoint = FloorIter->GetSpawnPoint();
		}
	}
	this->TimeSinceLastSpawn = this->SpawnTimer;
}

// Called every frame
void AObstacleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->TimeSinceLastSpawn += DeltaTime;

	float trueSpawnTime = this->SpawnTimer / (float)GetCustomGameMode<ABountyDashGameMode>(GetWorld())->GetGameLevel();

	if (this->TimeSinceLastSpawn > trueSpawnTime)
	{
		this->TimeSinceLastSpawn = 0.0f;
		SpawnObstacle();
	}
}

void AObstacleSpawner::SpawnObstacle()
{
	if (this->SpawnTargets.Num() > 0 && this->ObstaclesToSpawn.Num() > 0)
	{
		short Spawner = FMath::Rand() % this->SpawnTargets.Num();
		short Obstical = FMath::Rand() % this->ObstaclesToSpawn.Num();
		float CapsuleOffset = 0.0f;

		FActorSpawnParameters SpawnInfo;

		FTransform myTrans = this->SpawnTargets[Spawner]->GetTransform();
		myTrans.SetLocation(FVector(this->SpawnPoint, myTrans.GetLocation().Y, myTrans.GetLocation().Z));

		AObstacle* newObs = GetWorld()->SpawnActor<AObstacle>(ObstaclesToSpawn[Obstical], myTrans, SpawnInfo);

		if (newObs)
		{
			newObs->SetKillPoint(this->KillPoint);
			USphereComponent* obsSphere = Cast<USphereComponent>(newObs->GetComponentByClass(USphereComponent::StaticClass()));

			if (obsSphere)
			{
				newObs->AddActorLocalOffset(FVector(0.0f, 0.0f, obsSphere->GetUnscaledSphereRadius()));
			}
		}
	}
}
