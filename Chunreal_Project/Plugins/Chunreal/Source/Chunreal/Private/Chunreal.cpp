//-----------------------------------------------------------------------------
// file: Chunreal.cpp
// desc: Chunreal plugin module definition.
//
// Template plugin code provided by Epic Games.
// 
// authors: Eito Murakami (https://ccrma.stanford.edu/~eitom/) and Ge Wang (https://ccrma.stanford.edu/~ge/)
// date: Spring 2023
//-----------------------------------------------------------------------------

#include "Chunreal.h"
#include "MetasoundFrontendRegistries.h"
#include "Chunreal/chuck/chuck_globals.h"
#include "Chunreal/chuck/chuck_vm.h"

#define LOCTEXT_NAMESPACE "FChunrealModule"

//Define custom log category "LogChunreal"
DEFINE_LOG_CATEGORY(LogChunreal);

void FChunrealModule::StartupModule()
{
    //Register MetaSound Nodes
    FMetasoundFrontendRegistryContainer::Get()->RegisterPendingNodes();
}

void FChunrealModule::ShutdownModule()
{
}

//static function to output custom log
void FChunrealModule::Log(FString message)
{
    UE_LOG(LogChunreal, Log, TEXT("%s"), *message);
}

//Receive message from Chuck
void FChunrealModule::printThisFromChuck(const char* msg)
{
    std::string m = msg;
    if (m.length() > 0) m[m.length() - 1] = '\0';
    FChunrealModule::Log(m.c_str());
}

//Store ChucK reference with ID
bool FChunrealModule::StoreChuckRef(ChucK* chuck, FString id)
{
    if (id.IsEmpty() || ChuckMap.Contains(id))
    {
        return false;
    }
    else
    {
        ChuckMap.Add(id, chuck);
        return true;
    }
}

//Remove ChucK reference with ID
bool FChunrealModule::RemoveChuckRef(FString id)
{
    if(id.IsEmpty() || !ChuckMap.Contains(id))
    {
        return false;
    }
    else
    {
        ChuckMap.Remove(id);
        return true;
    }
}

//Get global int
int FChunrealModule::GetChuckGlobalInt(FString id, FString paramName)
{
    if (!ChuckMap.Contains(id))
    {
        return 0;
    }
    else
    {
        ChucK* chuck = *ChuckMap.Find(id);
        return chuck->globals()->get_global_int_value(TCHAR_TO_ANSI(*paramName));;
    }
}
//Set global int
bool FChunrealModule::SetChuckGlobalInt(FString id, FString paramName, int val)
{
    if (!ChuckMap.Contains(id))
    {
        return false;
    }
    else
    {
        ChucK* chuck = *ChuckMap.Find(id);
        chuck->globals()->setGlobalInt(TCHAR_TO_ANSI(*paramName), val);
        return true;
    }
}

//Get global float
float FChunrealModule::GetChuckGlobalFloat(FString id, FString paramName)
{
    if (!ChuckMap.Contains(id))
    {
        return 0;
    }
    else
    {
        ChucK* chuck = *ChuckMap.Find(id);
        return chuck->globals()->get_global_float_value(TCHAR_TO_ANSI(*paramName));;
    }
}
//Set global float
bool FChunrealModule::SetChuckGlobalFloat(FString id, FString paramName, float val)
{
    if (!ChuckMap.Contains(id))
    {
        return false;
    }
    else
    {
        ChucK* chuck = *ChuckMap.Find(id);
        chuck->globals()->setGlobalFloat(TCHAR_TO_ANSI(*paramName), val);
        return true;
    }
}

//Get global string
FString FChunrealModule::GetChuckGlobalString(FString id, FString paramName)
{
    if (!ChuckMap.Contains(id))
    {
        return "";
    }
    else
    {
        ChucK* chuck = *ChuckMap.Find(id);
        const char* string = chuck->globals()->get_global_string(TCHAR_TO_ANSI(*paramName))->str().c_str();
        return ANSI_TO_TCHAR(string);
    }
}
//Set global string
bool FChunrealModule::SetChuckGlobalString(FString id, FString paramName, FString val)
{
    if (!ChuckMap.Contains(id))
    {
        return false;
    }
    else
    {
        ChucK* chuck = *ChuckMap.Find(id);
        chuck->globals()->setGlobalString(TCHAR_TO_ANSI(*paramName), TCHAR_TO_ANSI(*val));
        return true;
    }
}

//Broadcast global event
bool FChunrealModule::BroadcastChuckGlobalEvent(FString id, FString paramName)
{
    if (!ChuckMap.Contains(id))
    {
        return false;
    }
    else
    {
        ChucK* chuck = *ChuckMap.Find(id);
        chuck->globals()->broadcastGlobalEvent(TCHAR_TO_ANSI(*paramName));
        return true;
    }
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FChunrealModule, Chunreal)
