// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/TriggerBox.h"
#include "LabyBotGoal.generated.h"

/**
 * 
 */
UCLASS()
class LABYBOT_API ALabyBotGoal : public ATriggerBox
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay();

public:
	ALabyBotGoal();

	UFUNCTION()
		void Event(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY(EditAnywhere)
};
