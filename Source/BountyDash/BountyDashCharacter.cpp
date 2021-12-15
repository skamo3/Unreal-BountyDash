// Fill out your copyright notice in the Description page of Project Settings.


#include "BountyDashCharacter.h"
#include "BountyDash.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimBlueprint.h"
#include "Engine/TargetPoint.h"
#include "BountyDashGameMode.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"
#include "Obstacle.h"


// Sets default values
ABountyDashCharacter::ABountyDashCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	ConstructorHelpers::FObjectFinder<UAnimBlueprint> myAnimBP(TEXT("/Game/Barrel_Hopper/Character/BH_Character_AnimeBP.BH_Character_AnimeBP"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> myMesh(TEXT("/Game/Barrel_Hopper/Character/Mesh/SK_Mannequin.SK_Mannequin"));

	if (myMesh.Succeeded() && myAnimBP.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(myMesh.Object);
		GetMesh()->SetAnimInstanceClass(myAnimBP.Object->GeneratedClass);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	GetCharacterMovement()->JumpZVelocity = 1450.0f;
	GetCharacterMovement()->GravityScale = 4.5f;

	this->CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	check(this->CameraBoom);

	this->CameraBoom->AttachTo(RootComponent);

	this->CameraBoom->TargetArmLength = 500.0f;

	this->CameraBoom->AddRelativeLocation(FVector(0.0f, 0.0f, 160.0f));

	this->FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	check(this->FollowCamera);

	this->FollowCamera->AttachTo(this->CameraBoom, USpringArmComponent::SocketName);

	this->FollowCamera->AddRelativeRotation(FQuat(FRotator(-10.0f, 0.0f, 0.0f)));


	this->CharSpeed = 10.0f;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABountyDashCharacter::MyOnComponentOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ABountyDashCharacter::MyOnComponentEndOverlap);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ABountyDashCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		TargetArray.Add(*TargetIter);
	}
	
	auto SortPred = [](const AActor& A, const AActor& B)->bool
	{
		return (A.GetActorLocation().Y < B.GetActorLocation().Y);
	};
	this->TargetArray.Sort(SortPred);
	this->CurrentLocation = ((TargetArray.Num() / 2) + (TargetArray.Num() % 2) - 1);
}

// Called every frame
void ABountyDashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetArray.Num() > 0)
	{
		FVector targetLoc = TargetArray[CurrentLocation]->GetActorLocation();
		targetLoc.Z = GetActorLocation().Z;
		targetLoc.X = GetActorLocation().X;

		if (targetLoc != GetActorLocation())
		{
			SetActorLocation(FMath::Lerp(GetActorLocation(), targetLoc, CharSpeed * DeltaTime));
		}
	}

	if (this->bBeingPushed)
	{
		float moveSpeed = GetCustomGameMode<ABountyDashGameMode>(GetWorld())->GetInvGameSpeed();
		AddActorLocalOffset(FVector(moveSpeed, 0.0f, 0.0f));
	}
}

// Called to bind functionality to input
void ABountyDashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("MoveRight", IE_Pressed, this, &ABountyDashCharacter::MoveRight);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &ABountyDashCharacter::MoveLeft);

}

void ABountyDashCharacter::MoveRight()
{
	if ((Controller != nullptr))
	{
		if (CurrentLocation < TargetArray.Num() - 1)
		{
			++CurrentLocation;
		}
		else
		{

		}
	}
}

void ABountyDashCharacter::MoveLeft()
{
	if (Controller != nullptr)
	{
		if (CurrentLocation > 0)
		{
			--CurrentLocation;
		}
		else
		{

		}
	}
}

void ABountyDashCharacter::MyOnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->IsChildOf(AObstacle::StaticClass()))
	{
		FVector vecBetween = OtherActor->GetActorLocation() - GetActorLocation();
		float AngleBetween = FMath::Acos(FVector::DotProduct(vecBetween.GetSafeNormal(), GetActorForwardVector().GetSafeNormal()));

		AngleBetween *= (180 / PI);
		if (AngleBetween < 60.0f)
		{
			this->bBeingPushed = true;
		}
	}
}

void ABountyDashCharacter::MyOnComponentEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->IsChildOf(AObstacle::StaticClass()))
	{
		this->bBeingPushed = false;
	}
}

void ABountyDashCharacter::ScoreUp()
{

}