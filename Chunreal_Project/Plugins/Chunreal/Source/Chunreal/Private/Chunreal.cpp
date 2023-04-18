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

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FChunrealModule, Chunreal)
