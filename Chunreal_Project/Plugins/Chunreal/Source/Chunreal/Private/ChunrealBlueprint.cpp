//-----------------------------------------------------------------------------
// file: ChunrealBlueprint.cpp
// desc: Chunreal blueprint utility functions definition.
//
// Template plugin code provided by Epic Games.
// 
// authors: Eito Murakami (https://ccrma.stanford.edu/~eitom/) and Ge Wang (https://ccrma.stanford.edu/~ge/)
// date: Spring 2023
//-----------------------------------------------------------------------------

#include "ChunrealBlueprint.h"

//Get ChucK sample rate
int UChunrealBlueprint::GetChuckSampleRate()
{
    return FChunrealModule::GetChuckSampleRate();
}

//Set ChucK sample rate
void UChunrealBlueprint::SetChuckSampleRate(int sampleRate)
{
    FChunrealModule::SetChuckSampleRate(sampleRate);
}

//Read file
bool UChunrealBlueprint::ReadFile(FString filepath, FString& text)
{
	return FFileHelper::LoadFileToString(text, *filepath);
}

//Get ChucK global int variable
int UChunrealBlueprint::GetChuckGlobalInt(FString id, FString paramName)
{
	return FChunrealModule::GetChuckGlobalInt(id, paramName);
}
//Set ChucK global int variable
bool UChunrealBlueprint::SetChuckGlobalInt(FString id, FString paramName, int val)
{
	return FChunrealModule::SetChuckGlobalInt(id, paramName, val);
}
//Get ChucK global float variable
float UChunrealBlueprint::GetChuckGlobalFloat(FString id, FString paramName)
{
	return FChunrealModule::GetChuckGlobalFloat(id, paramName);
}
//Set ChucK global float variable
bool UChunrealBlueprint::SetChuckGlobalFloat(FString id, FString paramName, float val)
{
	return FChunrealModule::SetChuckGlobalFloat(id, paramName, val);
}
//Get ChucK global string variable
FString UChunrealBlueprint::GetChuckGlobalString(FString id, FString paramName)
{
	return FChunrealModule::GetChuckGlobalString(id, paramName);
}
//Set ChucK global string variable
bool UChunrealBlueprint::SetChuckGlobalString(FString id, FString paramName, FString val)
{
	return FChunrealModule::SetChuckGlobalString(id, paramName, val);
}
//Set ChucK global int array variable
bool UChunrealBlueprint::SetChuckGlobalIntArray(FString id, FString paramName, TArray<int64> intArray, int arraySize)
{
	return FChunrealModule::SetChuckGlobalIntArray(id, paramName, (t_CKINT *)intArray.GetData(), arraySize);
}
//Set ChucK global float array variable
bool UChunrealBlueprint::SetChuckGlobalFloatArray(FString id, FString paramName, TArray<double> floatArray, int arraySize)
{
	return FChunrealModule::SetChuckGlobalFloatArray(id, paramName, floatArray.GetData(), arraySize);
}
//Broadcast ChucK global event
bool UChunrealBlueprint::BroadcastChuckGlobalEvent(FString id, FString paramName)
{
	return FChunrealModule::BroadcastChuckGlobalEvent(id, paramName);
}
