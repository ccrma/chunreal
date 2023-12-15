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
#include "Chunreal/chuck/chuck_globals.h"
#include "Chunreal/chuck/chuck_vm.h"

//whether to print chuck log
#define PRINT_CHUCK_LOG true

//Declare custom log category "LogChunreal"
DECLARE_LOG_CATEGORY_EXTERN(LogChunreal, Log, All);

class FChunrealModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    //reference to ChucK parent
    inline static ChucK* chuckParent = nullptr;

    //Get ChucK sample rate
    static t_CKINT GetChuckSampleRate();
    //Set ChucK sample rate
    static void SetChuckSampleRate(t_CKINT sampleRate);
    
    //static function to output custom log
    static void Log(FString message);

    //Receive message from Chuck with a mutex
    static void printThisFromChuck(const char* msg);

    //Compile ChucK code with a mutex
    static void CompileChuckCode(ChucK* chuckRef, const std::string& code, std::vector<t_CKUINT>* shredIDs = nullptr);

    //Run ChucK with a mutex
    static void RunChuck(ChucK* chuckRef, const float* input, float* output, t_CKINT numFrames);

    //Store and Remove ChucK ref with a mutex
    static bool StoreChuckRef(ChucK* chuck, FString id);
    static bool RemoveChuckRef(FString id);

    //Get next available ChucK sub index
    static int GetChuckSubIndex();
    
    //Global int
    static int GetChuckGlobalInt(FString id, FString paramName);
    static bool SetChuckGlobalInt(FString id, FString paramName, t_CKINT val);
    //Global float
    static float GetChuckGlobalFloat(FString id, FString paramName);
    static bool SetChuckGlobalFloat(FString id, FString paramName, t_CKFLOAT val);
    //Global string
    static FString GetChuckGlobalString(FString id, FString paramName);
    static bool SetChuckGlobalString(FString id, FString paramName, FString val);
    //Global int array
    static bool SetChuckGlobalIntArray(FString id, FString paramName, t_CKINT intArray[], t_CKUINT arraySize);
    //Global float array
    static bool SetChuckGlobalFloatArray(FString id, FString paramName, t_CKFLOAT floatArray[], t_CKUINT arraySize);
    //Global event
    static bool BroadcastChuckGlobalEvent(FString id, FString paramName);

    //TODO-global array getters & event listener

private:
    inline static t_CKINT chuckSampleRate = 44100;
    inline static TMap<FString, ChucK*> ChuckMap;
    inline static int subIndex = 0;

    inline static FCriticalSection printMutex;
    inline static FCriticalSection refMutex;
    inline static FCriticalSection compilerMutex;
    inline static FCriticalSection runMutex;
};
