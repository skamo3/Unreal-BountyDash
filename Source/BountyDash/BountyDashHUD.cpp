// Fill out your copyright notice in the Description page of Project Settings.


#include "BountyDashHUD.h"
#include "Engine/Font.h"
#include "BountyDashCharacter.h"
#include "BountyDashGameMode.h"
#include "Engine/Canvas.h"
#include "BountyDash.h"
#include "Kismet/GameplayStatics.h"

ABountyDashHUD::ABountyDashHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> OurHudFont(TEXT("/Game/BountyDashFont.BountyDashFont"));
	HUDFont = OurHudFont.Object;	
}

void ABountyDashHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

	ABountyDashCharacter* DashCharacter = Cast<ABountyDashCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	ABountyDashGameMode* DashGameMode = GetCustomGameMode<ABountyDashGameMode>(GetWorld());

	FString HUDString = FString::Printf(TEXT("Score: %d Runtime: %.4f"),
		DashCharacter->GetScore(), DashGameMode->GetRunTime());

	DrawText(HUDString, FColor::Yellow, 50, 50, HUDFont);
}