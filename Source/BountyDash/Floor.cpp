// Fill out your copyright notice in the Description page of Project Settings.

#include "Floor.h"
#include "BountyDash.h"
#include "BountyDashGameMode.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> myMesh(TEXT("/Game/Geometry/Meshes/Floor_Mesh_BountyDash.Floor_Mesh_BountyDash"));

	ConstructorHelpers::FObjectFinder<UMaterial> myMaterial(TEXT("/Game/StarterContent/Materials/M_Concrete_Tiles.M_Concrete_Tiles"));

	if (myMesh.Succeeded())
	{
		this->NumRepeatingMesh = 3;

		FBoxSphereBounds myBounds = myMesh.Object->GetBounds();
		float XBounds = myBounds.BoxExtent.X * 2;
		float ScenePos = ((XBounds * (this->NumRepeatingMesh - 1)) / 2.0f) * -1;

		this->KillPoint = ScenePos - (XBounds * 0.5f);
		this->SpawnPoint = (ScenePos * -1) + (XBounds * 0.5f);

		for (int i = 0; i < this->NumRepeatingMesh; ++i)
		{
			FString SceneName = "Scene" + FString::FromInt(i);
			FName SceneID = FName(*SceneName);
			USceneComponent* thisScene = CreateDefaultSubobject<USceneComponent>(SceneID);
			check(thisScene);
			thisScene->AttachTo(RootComponent);
			thisScene->SetRelativeLocation(FVector(ScenePos, 0.0f, 0.0f));
			ScenePos += XBounds;

			this->FloorMeshScenes.Add(thisScene);

			FString MeshName = "Mesh" + FString::FromInt(i);
			UStaticMeshComponent* thisMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(*MeshName));
			check(thisMesh);

			thisMesh->AttachTo(this->FloorMeshScenes[i]);
			thisMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
			thisMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

			if (myMaterial.Succeeded())
			{
				thisMesh->SetStaticMesh(myMesh.Object);
				thisMesh->SetMaterial(0, myMaterial.Object);
			}
			this->FloorMeshes.Add(thisMesh);
		}

		this->CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
		check(this->CollisionBox);

		this->CollisionBox->AttachTo(RootComponent);
		this->CollisionBox->SetBoxExtent(FVector(this->SpawnPoint, myBounds.BoxExtent.Y, myBounds.BoxExtent.Z));
		this->CollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	}

}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

float AFloor::GetKillPoint()
{
	return this->KillPoint;
}

float AFloor::GetSpawnPoint()
{
	return this->SpawnPoint;
}

// Called every frame
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (auto Scene : this->FloorMeshScenes)
	{
		Scene->AddLocalOffset(FVector(GetCustomGameMode<ABountyDashGameMode>(GetWorld())->GetInvGameSpeed(), 0.0f, 0.0f));

		if (Scene->GetComponentTransform().GetLocation().X <= this->KillPoint)
		{
			Scene->SetRelativeLocation(FVector(this->SpawnPoint, 0.0f, 0.0f));
		}
	}

}
