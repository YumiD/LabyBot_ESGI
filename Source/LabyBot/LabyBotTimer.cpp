// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyBotTimer.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, String)

// Sets default values
ALabyBotTimer::ALabyBotTimer() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_CallTracker = 3;
	_CallTrackerPeriod = 3.0f;
	_CallTrackerPrint = true;
}

ALabyBotTimer::ALabyBotTimer(int32 calltracker, float period, bool print)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_CallTracker = calltracker;
	_CallTrackerPeriod = period;
	_CallTrackerPrint = print;

}

void ALabyBotTimer::InitTimer(UInGameHUD& timer)
{
	Timer = &timer;
}

void ALabyBotTimer::StartTimer()
{
	GetWorldTimerManager().SetTimer(TimeHandle, this, &ALabyBotTimer::TimerFunction, 1.0f, true, 0.5f);
}

int32 ALabyBotTimer::GetMaxTime()
{
	return CallTracker;
}

void ALabyBotTimer::TimerFunction() {
	// Track the number of times we're calling the function
	CallTracker++;
	
	// If CallTracker is zero then print A and Clear the Timer to stop it looping. Otherwise print B
	if (CallTracker == 0) {
		PrintString("Finished Looping");
		GetWorldTimerManager().ClearTimer(TimeHandle);
		Timer->UpdateTextBlock(FString::FromInt(CallTracker));
	}
	else {
		PrintString("Timer Called");
		Timer->UpdateTextBlock(FString::FromInt(CallTracker));
	}

	// Print the number of loops the tracker has remaining
	if (_CallTrackerPrint)
		PrintString(FString::Printf(TEXT("Calls Remaining: %d"), _CallTracker));
}
