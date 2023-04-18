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
};
