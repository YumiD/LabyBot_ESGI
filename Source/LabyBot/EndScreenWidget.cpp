// Fill out your copyright notice in the Description page of Project Settings.


#include "EndScreenWidget.h"

void UEndScreenWidget::UpdateEndText(bool isVictory)
{
	EndText->SetText(FText::FromString(isVictory ? "You Win !" : "You Lose !"));
}
