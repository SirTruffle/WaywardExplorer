// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/* This function chops a string into an array of strings based on a specified delimiter.
SourceExample: Val1|Val2|Val3
OutputExample: {Val1, Val2, Val3}
*/
TArray<FString> ChopString(FString Source, char split = '|');
