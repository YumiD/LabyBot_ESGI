// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"
#include <string.h>
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"

class ALabyBotTimer;
#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, String)
UInGameHUD::UInGameHUD(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerPawn = Cast<ALabyBotPawn>(UGameplayStatics::GetPlayerPawn(UUserWidget::GetWorld(), 0));
}

void UInGameHUD::StartGame()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Start!"));
	PlayerPawn->Started = true;
	PlayerPawn->InitBattery();
	Timer->StartTimer();
	ImageLevel->SetVisibility(ESlateVisibility::Hidden);
	Crossroad1->SetVisibility(ESlateVisibility::Hidden);
}

void UInGameHUD::OnEndScreen(bool isVictory)
{
	if (EndScreen == nullptr)
		return;
	EndScreen->AddToViewport(); // Add it to the viewport so the Construct() method in the UUserWidget:: is run.
	EndScreen->UpdateEndText(isVictory);
}

void UInGameHUD::UpdateHUD(FString Time) const
{
	TimeText->SetText(FText::AsCultureInvariant(Time));
	EnergyBar->SetPercent(static_cast<float>(PlayerPawn->GetCurrentBattery()) / PlayerPawn->MaxBattery);
}

void UInGameHUD::UpdateCrossroad()
{
	switch (Crossroad1Direction) {
		case DirectionPawn::None:
			Crossroad1Text->SetText(FText::AsCultureInvariant("U"));
			Crossroad1Direction = DirectionPawn::Up;
			break;
		case DirectionPawn::Up:
			Crossroad1Text->SetText(FText::AsCultureInvariant("R"));
			Crossroad1Direction = DirectionPawn::Right;
			break;
		case DirectionPawn::Right:
			Crossroad1Text->SetText(FText::AsCultureInvariant("D"));
			Crossroad1Direction = DirectionPawn::Down;
			break;
		case DirectionPawn::Down:
			Crossroad1Text->SetText(FText::AsCultureInvariant("L"));
			Crossroad1Direction = DirectionPawn::Left;
			break;
		case DirectionPawn::Left:
			Crossroad1Text->SetText(FText::AsCultureInvariant(""));
			Crossroad1Direction = DirectionPawn::None;
			break;
		default:
			break;
	}
}

void UInGameHUD::NativeConstruct()
{
	Super::NativeConstruct();
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	Timer = GetWorld()->SpawnActor<ALabyBotTimer>(Location, Rotation, SpawnInfo);
	Timer->InitTimer(*this);
	TimeText->SetText(FText::AsCultureInvariant(FString::FromInt(Timer->GetMaxTime())));
	EnergyBar->SetPercent(100);
	PlayerPawn->PlayerEnd.AddDynamic(this, &UInGameHUD::OnEndScreen);
	EndScreen = CreateWidget<UEndScreenWidget>(GetWorld(), EndScreenClass);
}
