// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyBotCrossroad.h"

ALabyBotCrossroad::ALabyBotCrossroad() {
	OnActorBeginOverlap.AddDynamic(this, &ALabyBotCrossroad::Event);
}

void ALabyBotCrossroad::BeginPlay()
{
	Super::BeginPlay();
}

void ALabyBotCrossroad::Event(class AActor* overlappedActor, class AActor* otherActor) {
	ALabyBotPawn* pawn = Cast<ALabyBotPawn>(otherActor);
	if (pawn != NULL) {
		pawn->SetDirectionPawn(directionSelection);
	}
	//GetWorldTimerManager().SetTimer(TimeHandle, this, &ALabyBotCrossroad::ChangeDirection(otherActor), 3.0f, false, 1.0f);
}

void ALabyBotCrossroad::ChangeDirection(class AActor* otherActor) {
}