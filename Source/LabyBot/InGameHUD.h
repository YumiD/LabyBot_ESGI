// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LabyBotPawn.h"
#include "LabyBotTimer.h"
#include "EndScreenWidget.h"
#include "LabyBotCrossroad.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InGameHUD.generated.h"

class ALabyBotTimer;
class ALabyBotPawn;
class ALabyBotCrossroad;
class UEndScreenWidget;

/**
 *
 */
UCLASS()
class LABYBOT_API UInGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UInGameHUD(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(meta = (BindWidget))
		UTextBlock* TimeText;

	UFUNCTION(BlueprintCallable)
		void StartGame();

	UFUNCTION(BlueprintCallable)
		void UpdateCrossroad(int i);

	UPROPERTY(meta = (BindWidget))
		UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* StartText;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* EnergyBar;

	UPROPERTY(meta = (BindWidget))
		UImage* ImageLevel;

	UPROPERTY(meta = (BindWidget))
		UButton* Crossroad1;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Crossroad1Text;

	UPROPERTY(EditAnywhere, Category = "HUD")
		TSubclassOf<class UEndScreenWidget> EndScreenClass;

	UFUNCTION()
		void OnEndScreen(bool isVictory);

	void UpdateHUD(FString Time) const;
	int32 CallTracker = 60;
	TArray<ALabyBotCrossroad*> crossRoads;

	virtual void NativeConstruct() override;

private:
	ALabyBotTimer* Timer;
	ALabyBotPawn* PlayerPawn;
	ALabyBotCrossroad* Crossroad1Object;
	DirectionPawn Crossroad1Direction = DirectionPawn::None;
	UEndScreenWidget* EndScreen;
	bool isOver = false;
};
