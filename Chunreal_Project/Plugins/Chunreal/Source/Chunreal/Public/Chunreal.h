//-----------------------------------------------------------------------------
// file: Chunreal.h
// desc: Chunreal plugin module header.
//
// Template plugin code provided by Epic Games.
// 
// authors: Eito Murakami (https://ccrma.stanford.edu/~eitom/) and Ge Wang (https://ccrma.stanford.edu/~ge/)
// date: Spring 2023
//-----------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Chunreal/chuck/chuck.h"
#include "Chunreal/chuck/chuck_def.h"

//whether to print chuck log
#define PRINT_CHUCK_LOG false

//Declare custom log category "LogChunreal"
DECLARE_LOG_CATEGORY_EXTERN(LogChunreal, Log, All);

class FChunrealModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    //static function to output custom log
    static void Log(FString message);

    //Receive message from Chuck
    static void printThisFromChuck(const char* msg);

    //Store and Remove ChucK ref
    static bool StoreChuckRef(ChucK* chuck, FString id);
    static bool RemoveChuckRef(FString id);

    //Global int
    static int GetChuckGlobalInt(FString id, FString paramName);
    static bool SetChuckGlobalInt(FString id, FString paramName, int val);
    //Global float
    static float GetChuckGlobalFloat(FString id, FString paramName);
    static bool SetChuckGlobalFloat(FString id, FString paramName, float val);
    //Global string
    static FString GetChuckGlobalString(FString id, FString paramName);
    static bool SetChuckGlobalString(FString id, FString paramName, FString val);
    //Global event
    static bool BroadcastChuckGlobalEvent(FString id, FString paramName);
    //TODO-global arrays & event listener

private:
    inline static TMap<FString, ChucK*> ChuckMap;
};
