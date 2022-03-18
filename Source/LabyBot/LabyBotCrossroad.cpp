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
}

void ALabyBotCrossroad::UpdateDirection(DirectionPawn direction) {
	currentDirectionPawn = direction;
}