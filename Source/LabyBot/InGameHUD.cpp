// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"
#include <string.h>
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include <string>

class ALabyBotTimer;
#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, String)
UInGameHUD::UInGameHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerPawn = Cast<ALabyBotPawn>(UGameplayStatics::GetPlayerPawn(UUserWidget::GetWorld(), 0));

	UWorld* world = GetWorld();
	for (TObjectIterator<ALabyBotCrossroad> It; It; ++It)
	{
		ALabyBotCrossroad* crossRoad = *It;
		if (crossRoad->GetWorld() == world) {
			crossRoads.Add(crossRoad);
		}
	}
	crossRoads.Sort();
}

void UInGameHUD::StartGame()
{
	if (PlayerPawn->Started && isOver)
	{
		UGameplayStatics::OpenLevel(this, FName("Level1"));
	}
	PlayerPawn->Started = true;
	StartText->SetText(FText::FromString("Restart"));
	StartButton->SetVisibility(ESlateVisibility::Hidden);
	PlayerPawn->InitBattery();
	Timer->StartTimer();
	ImageLevel->SetVisibility(ESlateVisibility::Hidden);
	for (int i = 0; i < CrossroadsButtons.Num(); i++) {
		CrossroadsButtons[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInGameHUD::OnEndScreen(bool isVictory)
{
	if (EndScreen == nullptr)
		return;
	StartButton->SetVisibility(ESlateVisibility::Visible);
	EndScreen->AddToViewport();
	EndScreen->UpdateEndText(isVictory);
	isOver = true;
}

void UInGameHUD::UpdateHUD(FString Time) const
{
	TimeText->SetText(FText::AsCultureInvariant(Time));
	EnergyBar->SetPercent(static_cast<float>(PlayerPawn->GetCurrentBattery()) / PlayerPawn->MaxBattery);
}

void UInGameHUD::UpdateCrossroad(int i)
{
	if (!VerifyJoker(CrossroadsText[i]->GetText().ToString())) return;
	switch (CrossroadsDirections[i]) {
	case DirectionPawn::None:
		CrossroadsText[i]->SetText(FText::AsCultureInvariant("U"));
		CrossroadsDirections[i] = DirectionPawn::Up;
		crossRoads[i]->UpdateDirection(DirectionPawn::Up);
		break;
	case DirectionPawn::Up:
		CrossroadsText[i]->SetText(FText::AsCultureInvariant("R"));
		CrossroadsDirections[i] = DirectionPawn::Right;
		crossRoads[i]->UpdateDirection(DirectionPawn::Right);
		break;
	case DirectionPawn::Right:
		CrossroadsText[i]->SetText(FText::AsCultureInvariant("D"));
		CrossroadsDirections[i] = DirectionPawn::Down;
		crossRoads[i]->UpdateDirection(DirectionPawn::Down);
		break;
	case DirectionPawn::Down:
		CrossroadsText[i]->SetText(FText::AsCultureInvariant("L"));
		CrossroadsDirections[i] = DirectionPawn::Left;
		crossRoads[i]->UpdateDirection(DirectionPawn::Left);
		break;
	case DirectionPawn::Left:
		CrossroadsText[i]->SetText(FText::AsCultureInvariant(""));
		CrossroadsDirections[i] = DirectionPawn::None;
		break;
	default:
		break;
	}
}

bool UInGameHUD::VerifyJoker(FString direction)
{
	int count = 0;
	for (size_t i = 0; i < CrossroadsText.Num(); i++)
	{
		if (!CrossroadsText[i]->GetText().ToString().IsEmpty() && !CrossroadsText[i]->GetText().ToString().Contains(direction)) {
			count++;
		}
		if (count >= MaxJokers)
		{
			return false;
		}
	}
	return true;
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
	GetAllButtons();
	GetAllText();
	for (int i = 0; i < CrossroadsButtons.Num(); i++) {
		CrossroadsDirections.Add(DirectionPawn::None);
	}
}

void UInGameHUD::GetAllButtons()
{
	const UWorld* world = GetWorld();

	FString cross = "Crossroad";
	for (TObjectIterator<UButton> Itr; Itr; ++Itr)
	{
		UButton* liveButton = *Itr;
		if (liveButton->GetWorld() != world)
		{
			continue;
		}
		if (liveButton->GetFName().ToString().Contains(cross))
		{
			CrossroadsButtons.Add(liveButton);
		}
	}
}

void UInGameHUD::GetAllText()
{
	const UWorld* world = GetWorld();

	FString cross = "Crossroad";
	for (TObjectIterator<UTextBlock> Itr; Itr; ++Itr)
	{
		UTextBlock* liveText = *Itr;
		if (liveText->GetWorld() != world)
		{
			continue;
		}
		if (liveText->GetFName().ToString().Contains(cross))
		{
			CrossroadsText.Add(liveText);
		}
	}
}
