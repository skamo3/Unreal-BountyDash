// Fill out your copyright notice in the Description page of Project Settings.


#include "BountyDashGameMode.h"
#include "BountyDashCharacter.h"
#include "BountyDashHUD.h"

ABountyDashGameMode::ABountyDashGameMode()
{
	DefaultPawnClass = ABountyDashCharacter::StaticClass();

	this->numCoinsForSpeedIncrease = 5;
	this->gameSpeed = 10.0f;
	this->gameSpeedIncrease = 5.0f;
	this->gameLevel = 1;
	HUDClass = ABountyDashHUD::StaticClass();

	RunTime = 0.0f;
	bGameOver = false;
	startGameOverCount = false;
	timeTillGameOver = 2.0f;
	gameOverTimer = 0.0f;
}


void ABountyDashGameMode::CharScoreUp(unsigned int charScore)
{
	if (charScore != 0 && charScore % this->numCoinsForSpeedIncrease == 0)
	{
		this->gameSpeed += this->gameSpeedIncrease;
		this->gameLevel++;
	}
}

float ABountyDashGameMode::GetInvGameSpeed()
{
	return -gameSpeed;
}

float ABountyDashGameMode::GetGameSpeed()
{
	return this->gameSpeed;
}


int32 ABountyDashGameMode::GetGameLevel()
{
	return this->gameLevel;
}

void ABountyDashGameMode::ReduceGameSpeed()
{
	if (gameSpeed > 10.0f)
	{
		gameSpeed -= gameSpeedIncrease;
		gameLevel--;
	}
}

void ABountyDashGameMode::Tick(float DeltaTime)
{
	if (!startGameOverCount)
	{
		RunTime += DeltaTime;
	}
	else
	{
		gameOverTimer += DeltaTime;
		if (gameOverTimer >= timeTillGameOver)
		{
			bGameOver = true;
		}
	}
}

float ABountyDashGameMode::GetRunTime()
{
	return RunTime;
}

bool ABountyDashGameMode::GetGameOver()
{
	return bGameOver;
}

void ABountyDashGameMode::GameOver()
{
	startGameOverCount = true;
}

void ABountyDashGameMode::SetGamePaused(bool gamePaused)
{
	APlayerController* myPlayer = GetWorld()->GetFirstPlayerController();
	if (myPlayer != nullptr)
	{
		myPlayer->SetPause(gamePaused);
	}
}
