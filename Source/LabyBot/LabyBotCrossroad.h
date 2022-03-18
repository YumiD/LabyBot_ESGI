// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LabyBotPawn.h"
#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "LabyBotCrossroad.generated.h"

/**
 * 
 */
UCLASS()
class LABYBOT_API ALabyBotCrossroad : public ATriggerBox
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay();

public:
	ALabyBotCrossroad();

	void UpdateDirection(DirectionPawn direction);

	UFUNCTION()
		void Event(class AActor* overlappedActor, class AActor* otherActor);

	//TODO Mettre des particules
	UPROPERTY(EditAnywhere)
		DirectionPawn directionSelection;

private:
	DirectionPawn currentDirectionPawn;

	FTimerHandle TimeHandle;
	
};
