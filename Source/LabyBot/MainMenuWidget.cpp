// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"

bool UMainMenuWidget::Initialize()
{
	//Super::Initialize();
	StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClickStart);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnClickQuit);
	AddToViewport(0);
	return true;
}

void UMainMenuWidget::OnClickStart()
{
	UGameplayStatics::OpenLevel(this, FName("Level1"));
}

void UMainMenuWidget::OnClickQuit()
{
	FGenericPlatformMisc::RequestExit(true);
}