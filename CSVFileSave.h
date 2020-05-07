// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CSVFileSave.generated.h"

/**
 * 

 This code can be found at https://www.youtube.com/watch?time_continue=231&v=uZPzTN5Debc&feature=emb_logo


 */
UCLASS()
class CTPRO870178_API UCSVFileSave : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

        UFUNCTION(BlueprintCallable, Category = "Custom", meta = (Keywords = "Save"))
        static bool SaveArrayText(FString SaveDirectory, FString FileName, TArray<FString> SaveText, bool AllowOverwriting);


};
