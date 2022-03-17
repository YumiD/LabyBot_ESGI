// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyBotGoal.h"

ALabyBotGoal::ALabyBotGoal() {
	OnActorBeginOverlap.AddDynamic(this, &ALabyBotGoal::Event);
}

void ALabyBotGoal::BeginPlay()
{
	Super::BeginPlay();
}

void ALabyBotGoal::Event(class AActor* overlappedActor, class AActor* otherActor) {
	/*if (otherActor && otherActor != this) {
		UE_LOG(LogTemp, Warning, TEXT("GOAL!!!"));
	}*/
	ALabyBotPawn* pawn = Cast<ALabyBotPawn>(otherActor);

	if (pawn != NULL) {
		pawn->Goal();
	}
}

