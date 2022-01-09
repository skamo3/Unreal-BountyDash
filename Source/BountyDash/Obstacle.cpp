// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "BountyDashGameMode.h"
#include "BountyDash.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	check(this->Collider);

	RootComponent = this->Collider;
	this->Collider->SetCollisionProfileName("OverlapAllDynamic");

	this->Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(this->Mesh);
	this->Mesh->AttachTo(this->Collider);
	this->Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	this->KillPoint = -20000.0f;

	OnActorBeginOverlap.AddDynamic(this, &AObstacle::MyOnActorOverlap);
	OnActorBeginOverlap.AddDynamic(this, &AObstacle::MyOnActorEndOverlap);
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float gameSpeed = GetCustomGameMode<ABountyDashGameMode>(GetWorld())->GetInvGameSpeed();

	AddActorWorldOffset(FVector(gameSpeed, 0.0f, 0.0f));

	if (GetActorLocation().X < this->KillPoint)
	{
		Destroy();
	}
}


void AObstacle::MyOnActorOverlap(AActor* OverlappedActor, AActor* otherActor)
{

}

void AObstacle::MyOnActorEndOverlap(AActor* OverlappedActor, AActor* otherActor)
{

}

void AObstacle::SetKillPoint(float point)
{
	this->KillPoint = point;
}

float AObstacle::GetKillPoint()
{
	return this->KillPoint;
}
