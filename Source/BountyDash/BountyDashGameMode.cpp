// Fill out your copyright notice in the Description page of Project Settings.


#include "BountyDashGameMode.h"
#include "BountyDashCharacter.h"

ABountyDashGameMode::ABountyDashGameMode()
{
	DefaultPawnClass = ABountyDashCharacter::StaticClass();

	this->numCoinsForSpeedIncrease = 5;
	this->gameSpeed = 10.0f;
	this->gameSpeedIncrease = 5.0f;
	this->gameLevel = 1;
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
