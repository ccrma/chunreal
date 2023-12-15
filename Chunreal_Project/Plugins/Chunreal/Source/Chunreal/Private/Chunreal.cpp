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
    //Create Chuck
    chuckParent = new ChucK();

    //Initialize Chuck params
    chuckParent->setParam(CHUCK_PARAM_SAMPLE_RATE, FChunrealModule::GetChuckSampleRate());
    chuckParent->setParam(CHUCK_PARAM_INPUT_CHANNELS, 2);
    chuckParent->setParam(CHUCK_PARAM_OUTPUT_CHANNELS, 2);
    chuckParent->setParam(CHUCK_PARAM_VM_ADAPTIVE, 0);
    chuckParent->setParam(CHUCK_PARAM_VM_HALT, (t_CKINT)(false));
    //chuckParent->setParam(CHUCK_PARAM_OTF_PORT, g_otf_port);
    //chuckParent->setParam(CHUCK_PARAM_OTF_ENABLE, (t_CKINT)TRUE);
    //chuckParent->setParam(CHUCK_PARAM_DUMP_INSTRUCTIONS, (t_CKINT)dump);
    chuckParent->setParam(CHUCK_PARAM_AUTO_DEPEND, (t_CKINT)0);
    //chuckParent->setParam(CHUCK_PARAM_DEPRECATE_LEVEL, deprecate_level);
    chuckParent->setParam(CHUCK_PARAM_CHUGIN_ENABLE, false);
    //chuckParent->setParam(CHUCK_PARAM_USER_CHUGINS, named_dls);
    //chuckParent->setParam(CHUCK_PARAM_USER_CHUGIN_DIRECTORIES, dl_search_path);
    chuckParent->setParam(CHUCK_PARAM_HINT_IS_REALTIME_AUDIO, true);

    //Start ChucK parent
    chuckParent->init();
    chuckParent->start();

    //Set log
#if PRINT_CHUCK_LOG
    chuckParent->setStdoutCallback(printThisFromChuck);
    chuckParent->setStderrCallback(printThisFromChuck);
    //theChuck->setLogLevel(CK_LOG_INFO);
#endif

    //Store as "ChuckParent"
    StoreChuckRef(chuckParent, "ChuckParent");

    //Register MetaSound Nodes
    FMetasoundFrontendRegistryContainer::Get()->RegisterPendingNodes();
}

void FChunrealModule::ShutdownModule()
{
    //Clear array
    ChuckMap.Empty();

    //Delete ChucK parent
    delete chuckParent;
    chuckParent = nullptr;  
}

/// <summary>
/// Get ChucK sample rate
/// </summary>
/// <returns></returns>
t_CKINT FChunrealModule::GetChuckSampleRate()
{
    return chuckSampleRate;
}

/// <summary>
/// Set ChucK sample rate
/// </summary>
/// <param name="sampleRate"></param>
void FChunrealModule::SetChuckSampleRate(t_CKINT sampleRate)
{
    chuckSampleRate = sampleRate;
}

/// <summary>
/// static function to output custom log
/// </summary>
/// <param name="message"></param>
void FChunrealModule::Log(FString message)
{
    UE_LOG(LogChunreal, Log, TEXT("%s"), *message);
}

/// <summary>
/// Receive message from Chuck with a mutex
/// </summary>
/// <param name="msg"></param>
void FChunrealModule::printThisFromChuck(const char* msg)
{
    printMutex.Lock();
    std::string m = msg;
    if (m.length() > 0) m[m.length() - 1] = '\0';
    FChunrealModule::Log(m.c_str());
    printMutex.Unlock();
}

/// <summary>
/// Compile ChucK code with a mutex
/// </summary>
/// <param name="chuckRef"></param>
/// <param name="code"></param>
/// <param name="shredIDs"></param>
void FChunrealModule::CompileChuckCode(ChucK* chuckRef, const std::string& code, std::vector<t_CKUINT>* shredIDs)
{
    compilerMutex.Lock();
    chuckRef->compileCode(code, "", 1, false, shredIDs);
    compilerMutex.Unlock();
}

/// <summary>
/// Run ChucK with a mutex
/// </summary>
/// <param name="chuckRef"></param>
/// <param name="input"></param>
/// <param name="output"></param>
/// <param name="numFrames"></param>
void FChunrealModule::RunChuck(ChucK* chuckRef, const float* input, float* output, t_CKINT numFrames)
{
    runMutex.Lock();
    chuckRef->run(input, output, numFrames);
    runMutex.Unlock();
}

/// <summary>
/// Store ChucK reference with ID with a mutex
/// </summary>
/// <param name="chuck"></param>
/// <param name="id"></param>
/// <returns></returns>
bool FChunrealModule::StoreChuckRef(ChucK* chuck, FString id)
{
    refMutex.Lock();
    if (id.IsEmpty() || ChuckMap.Contains(id))
    {
        refMutex.Unlock();
        return false;
    }
    else
    {
        ChuckMap.Add(id, chuck);
        refMutex.Unlock();
        return true;
    }
}

/// <summary>
/// Remove ChucK reference with ID with a mutex
/// </summary>
/// <param name="id"></param>
/// <returns></returns>
bool FChunrealModule::RemoveChuckRef(FString id)
{
    refMutex.Lock();
    if(id.IsEmpty() || !ChuckMap.Contains(id))
    {
        refMutex.Unlock();
        return false;
    }
    else
    {
        ChuckMap.Remove(id);
        refMutex.Unlock();
        return true;
    }
}

/// <summary>
/// Get ChucK sub index
/// </summary>
/// <returns></returns>
int FChunrealModule::GetChuckSubIndex()
{
    return ++subIndex;
}

/// <summary>
/// Get global int
/// </summary>
/// <param name="id"></param>
/// <param name="paramName"></param>
/// <returns></returns>
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
/// <summary>
/// Set global int
/// </summary>
/// <param name="id"></param>
/// <param name="paramName"></param>
/// <param name="val"></param>
/// <returns></returns>
bool FChunrealModule::SetChuckGlobalInt(FString id, FString paramName, t_CKINT val)
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

/// <summary>
/// Get global float
/// </summary>
/// <param name="id"></param>
/// <param name="paramName"></param>
/// <returns></returns>
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
/// <summary>
/// Set global float
/// </summary>
/// <param name="id"></param>
/// <param name="paramName"></param>
/// <param name="val"></param>
/// <returns></returns>
bool FChunrealModule::SetChuckGlobalFloat(FString id, FString paramName, t_CKFLOAT val)
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

/// <summary>
/// Get global string
/// </summary>
/// <param name="id"></param>
/// <param name="paramName"></param>
/// <returns></returns>
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
/// <summary>
/// Set global string
/// </summary>
/// <param name="id"></param>
/// <param name="paramName"></param>
/// <param name="val"></param>
/// <returns></returns>
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

/// <summary>
/// Set global int array
/// </summary>
/// <param name="id"></param>
/// <param name="paramName"></param>
/// <param name="intArray"></param>
/// <param name="arraySize"></param>
/// <returns></returns>
bool FChunrealModule::SetChuckGlobalIntArray(FString id, FString paramName, t_CKINT intArray[], t_CKUINT arraySize)
{
    if (!ChuckMap.Contains(id))
    {
        return false;
    }
    else
    {
        ChucK* chuck = *ChuckMap.Find(id);
        chuck->globals()->setGlobalIntArray(TCHAR_TO_ANSI(*paramName), intArray, arraySize);
        return true;
    }
}

/// <summary>
/// Set global float array
/// </summary>
/// <param name="id"></param>
/// <param name="paramName"></param>
/// <param name="floatArray"></param>
/// <param name="arraySize"></param>
/// <returns></returns>
bool FChunrealModule::SetChuckGlobalFloatArray(FString id, FString paramName, t_CKFLOAT floatArray[], t_CKUINT arraySize)
{
    if (!ChuckMap.Contains(id))
    {
        return false;
    }
    else
    {
        ChucK* chuck = *ChuckMap.Find(id);
        chuck->globals()->setGlobalFloatArray(TCHAR_TO_ANSI(*paramName), floatArray, arraySize);
        return true;
    }
}

/// <summary>
/// Broadcast global event
/// </summary>
/// <param name="id"></param>
/// <param name="paramName"></param>
/// <returns></returns>
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
