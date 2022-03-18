// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyBotCrossroad.h"

#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, String)

ALabyBotCrossroad::ALabyBotCrossroad() {
	OnActorBeginOverlap.AddDynamic(this, &ALabyBotCrossroad::Event);
	currentDirectionPawn = directionSelection;
	isActive = true;
}

void ALabyBotCrossroad::BeginPlay()
{
	Super::BeginPlay();
}

void ALabyBotCrossroad::Event(class AActor* overlappedActor, class AActor* otherActor) {
	ALabyBotPawn* pawn = Cast<ALabyBotPawn>(otherActor);
	if (pawn != NULL && isActive) {
		if (currentDirectionPawn != DirectionPawn::None) {
			pawn->SetDirectionPawn(currentDirectionPawn);
			isActive = false;
		}
	}
}

void ALabyBotCrossroad::UpdateDirection(DirectionPawn direction) {
	currentDirectionPawn = direction;
}