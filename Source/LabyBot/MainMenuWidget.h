// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYBOT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
		UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
		UButton* ExitButton;

	UFUNCTION()
		void OnClickStart();
	UFUNCTION()
		void OnClickQuit();
};
