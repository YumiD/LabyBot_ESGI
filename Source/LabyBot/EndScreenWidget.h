// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EndScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYBOT_API UEndScreenWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* EndText;

	void UpdateEndText(bool isVictory);
};
