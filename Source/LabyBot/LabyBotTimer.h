// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LabyBotTimer.generated.h"

UCLASS()
class LABYBOT_API ALabyBotTimer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALabyBotTimer();

	ALabyBotTimer(int32 calltracker, float period, bool print);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Max Timer Loops
	int32 _CallTracker;

	// How often to call timer
	float _CallTrackerPeriod;

	// Do we print how many left?
	bool _CallTrackerPrint;

public:	

	// Timer Function
	void TimerFunction();

	// Timer Handle (For Control)
	FTimerHandle _TimeHandle;

	int32 getCallTracker();
};
