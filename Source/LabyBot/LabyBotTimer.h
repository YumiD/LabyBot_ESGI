// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGameHUD.h"
#include "GameFramework/Actor.h"
#include "LabyBotTimer.generated.h"

class UInGameHUD;

UCLASS()
class LABYBOT_API ALabyBotTimer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALabyBotTimer();
	void InitTimer(UInGameHUD& timer);
	void StartTimer();
	int32 GetMaxTime();
private:
	// Max Timer Loops
	int32 CallTracker = 0;
	UInGameHUD* Timer;

public:
	// Timer Function
	void TimerFunction();

	// Timer Handle (For Control)
	FTimerHandle TimeHandle;
};
