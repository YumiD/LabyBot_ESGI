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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Max Timer Loops
	int32 CallTracker = 3;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Timer Function
	void TimerFunction();

	// Timer Handle (For Control)
	FTimerHandle TimeHandle;

};
