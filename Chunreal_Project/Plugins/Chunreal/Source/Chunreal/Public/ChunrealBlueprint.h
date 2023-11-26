//-----------------------------------------------------------------------------
// file: ChunrealBlueprint.h
// desc: Chunreal blueprint utility functions header.
//
// Template plugin code provided by Epic Games.
// 
// authors: Eito Murakami (https://ccrma.stanford.edu/~eitom/) and Ge Wang (https://ccrma.stanford.edu/~ge/)
// date: Spring 2023
//-----------------------------------------------------------------------------

#pragma once

#include "Chunreal.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "ChunrealBlueprint.generated.h"

UCLASS()
class UChunrealBlueprint : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
    public:
        /**
         * Get ChucK sample rate
         */
        UFUNCTION(BlueprintPure, Category = "Chunreal", meta = (keywords = "Get ChucK sample rate"))
            static int GetChuckSampleRate();
        
        /**
        * Set ChucK sample rate
        * @param sampleRate
        */
        UFUNCTION(BlueprintCallable, Category = "Chunreal", meta = (keywords = "Set ChucK sample rate"))
            static void SetChuckSampleRate(int sampleRate = 44100);
    
        /**
        * Read file
        * @param filePath Absolute path to the file
        */
        UFUNCTION(BlueprintPure, Category = "Chunreal", meta = (keywords = "Read file"))
            static bool ReadFile(FString filePath, FString& text);

        /**
        * Get ChucK global int variable
        * @param ID ChucK ID
        * @param paramName Name of the ChucK global int variable
        */
        UFUNCTION(BlueprintCallable, Category = "Chunreal", meta = (keywords = "Get ChucK Int"))
            static int GetChuckGlobalInt(FString id, FString paramName);
        /**
        * Set ChucK global int variable
        * @param ID ChucK ID
        * @param paramName Name of the ChucK global int variable
        * @param val int value
        */
        UFUNCTION(BlueprintCallable, Category = "Chunreal", meta = (keywords = "Set ChucK Int"))
            static bool SetChuckGlobalInt(FString id, FString paramName, int val);
        /**
        * Get ChucK global float variable
        * @param ID ChucK ID
        * @param paramName Name of the ChucK global float variable
        */
        UFUNCTION(BlueprintCallable, Category = "Chunreal", meta = (keywords = "Get ChucK Float"))
            static float GetChuckGlobalFloat(FString id, FString paramName);
        /**
        * Set ChucK global float variable 
        * @param ID ChucK ID
        * @param paramName Name of the ChucK global float variable
        * @param val float value
        */
        UFUNCTION(BlueprintCallable, Category = "Chunreal", meta = (keywords = "Set ChucK Float"))
            static bool SetChuckGlobalFloat(FString id, FString paramName, float val);
        /**
        * Get ChucK global string variable
        * @param ID ChucK ID
        * @param paramName Name of the ChucK global string variable
        */
        UFUNCTION(BlueprintCallable, Category = "Chunreal", meta = (keywords = "Get ChucK String"))
            static FString GetChuckGlobalString(FString id, FString paramName);
        /**
        * Set ChucK global string variable
        * @param ID ChucK ID
        * @param paramName Name of the ChucK global string variable
        * @param val string
        */
        UFUNCTION(BlueprintCallable, Category = "Chunreal", meta = (keywords = "Set ChucK String"))
            static bool SetChuckGlobalString(FString id, FString paramName, FString val);

        /**
        * Set ChucK global int array variable
        * @param ID ChucK ID
        * @param paramName Name of the ChucK global string variable
        * @param intArray integer array
        * @param arraySize size of array
        */
        UFUNCTION(BlueprintCallable, Category = "Chunreal", meta = (keywords = "Set ChucK Int Array"))
            static bool SetChuckGlobalIntArray(FString id, FString paramName, TArray<int64> intArray, int arraySize);

        /**
        * Set ChucK global float array variable
        * @param ID ChucK ID
        * @param paramName Name of the ChucK global string variable
        * @param floatArray float array
        * @param arraySize size of array
        */
        UFUNCTION(BlueprintCallable, Category = "Chunreal", meta = (keywords = "Set ChucK Float Array"))
            static bool SetChuckGlobalFloatArray(FString id, FString paramName, TArray<double> floarArray, int arraySize);

        /**
        * Broadcast ChucK global event
        * @param ID ChucK ID
        * @param paramName Name of the ChucK global event to broadcast
        */
        UFUNCTION(BlueprintCallable, Category = "Chunreal", meta = (keywords = "Broadcast ChucK Event"))
            static bool BroadcastChuckGlobalEvent(FString id, FString paramName);
};
