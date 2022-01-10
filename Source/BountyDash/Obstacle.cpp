// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "BountyDashGameMode.h"
#include "BountyDash.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->Mesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("Mesh"));
	check(this->Mesh);
	this->Mesh->AttachTo(this->Collider);
	this->Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);

}

UDestructibleComponent* AObstacle::GetDestructable()
{
	return Mesh;
}