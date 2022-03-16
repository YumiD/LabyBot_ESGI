// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyBotTimer.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, String)

// Sets default values
ALabyBotTimer::ALabyBotTimer()
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

// Called when the game starts or when spawned
void ALabyBotTimer::BeginPlay()
{
	Super::BeginPlay();

	// Timer Setup : Call the TimerFunction every 3 seconds, ensure the timer loops and delay first call for half a second
	// 1st arg -> Timer Handle (For Control) 
	// 2nd arg -> Object on which function is located
	// 3rd arg -> Function to call on a timer
	// 4rd arg -> How often to call timer
	// 5rd arg -> Loop?
	// 6rd arg -> Time before first call
	GetWorldTimerManager().SetTimer(_TimeHandle, this, &ALabyBotTimer::TimerFunction, _CallTrackerPeriod, true, 1.0f);
	
}

void ALabyBotTimer::TimerFunction() {
	// Track the number of times we're calling the function
	_CallTracker--;
	
	// If CallTracker is zero then print A and Clear the Timer to stop it looping. Otherwise print B
	//CallTracker == 0 ? PrintString("Finished Looping"), GetWorldTimerManager().ClearTimer(TimeHandle) : PrintString("Timer Called");
	if (_CallTracker == 0) {
		if (_CallTrackerPrint)
			PrintString("Finished Looping");
		GetWorldTimerManager().ClearTimer(_TimeHandle);
		//this->Destroy();
	}
	else {
		if(_CallTrackerPrint)
			PrintString("Timer Called");
	}

	// Print the number of loops the tracker has remaining
	if (_CallTrackerPrint)
		PrintString(FString::Printf(TEXT("Calls Remaining: %d"), _CallTracker));
}

int32 ALabyBotTimer::getCallTracker() {
	return _CallTracker;
}