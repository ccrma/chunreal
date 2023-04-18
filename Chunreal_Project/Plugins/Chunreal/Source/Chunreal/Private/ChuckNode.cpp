//-----------------------------------------------------------------------------
// file: ChuckNode.cpp
// desc: Chuck MetaSound node definition.
//
// Template MetaSound code provided by Epic Games.
// 
// authors: Eito Murakami (https://ccrma.stanford.edu/~eitom/) and Ge Wang (https://ccrma.stanford.edu/~ge/)
// date: Spring 2023
//-----------------------------------------------------------------------------

#include "ChuckNode.h"
#include "Chunreal/chuck/chuck.h"

#define LOCTEXT_NAMESPACE "Metasound_ChuckNode"

namespace Metasound
{
    // TODO: move this somewhere sensible?
    static void printThisFromChuck(const char* msg)
    {
        std::string m = msg;
        if (m.length() > 0) m[m.length() - 1] = '\0';  
        FChunrealModule::Log(m.c_str());
    }

    //------------------------------------------------------------------------------------
    // FChuckOperator
    //------------------------------------------------------------------------------------
    FChuckOperator::FChuckOperator(const FOperatorSettings& InSettings, const FTriggerReadRef& InTrigger, const FStringReadRef& InCode, const FAudioBufferReadRef& InAudioInput, const FFloatReadRef& InAmplitude)
        : Trigger(InTrigger)
        , Code(InCode)
        , AudioInput(InAudioInput)
        , AudioOutput(FAudioBufferWriteRef::CreateNew(InSettings))
        , Amplitude(InAmplitude)
    { 
        //FChunrealModule::Log("ChuckNode created");

        //Create Chuck
        theChuck = new ChucK();

        #if PRINT_CHUCK_LOG
        //Set the error print destination
        theChuck->setStdoutCallback(printThisFromChuck);
        theChuck->setStderrCallback(printThisFromChuck);
        //Set chuck log level
        theChuck->setLogLevel(CK_LOG_INFO);
        #endif

        //Initialize Chuck params
        theChuck->setParam(CHUCK_PARAM_SAMPLE_RATE, 44100);
        theChuck->setParam(CHUCK_PARAM_INPUT_CHANNELS, 1);
        theChuck->setParam(CHUCK_PARAM_OUTPUT_CHANNELS, 1);
        theChuck->setParam(CHUCK_PARAM_VM_ADAPTIVE, 0);
        theChuck->setParam(CHUCK_PARAM_VM_HALT, (t_CKINT)(false));
        //theChuck->setParam(CHUCK_PARAM_OTF_PORT, g_otf_port);
        //theChuck->setParam(CHUCK_PARAM_OTF_ENABLE, (t_CKINT)TRUE);
        //theChuck->setParam(CHUCK_PARAM_DUMP_INSTRUCTIONS, (t_CKINT)dump);
        theChuck->setParam(CHUCK_PARAM_AUTO_DEPEND, (t_CKINT)0);
        //theChuck->setParam(CHUCK_PARAM_DEPRECATE_LEVEL, deprecate_level);
        theChuck->setParam(CHUCK_PARAM_CHUGIN_ENABLE, false);
        //theChuck->setParam(CHUCK_PARAM_USER_CHUGINS, named_dls);
        //theChuck->setParam(CHUCK_PARAM_USER_CHUGIN_DIRECTORIES, dl_search_path);
        theChuck->setParam(CHUCK_PARAM_HINT_IS_REALTIME_AUDIO, true);

        //Start Chuck
        theChuck->init();
        theChuck->start();
    }
    FChuckOperator::~FChuckOperator()
    {
        //FChunrealModule::Log("ChuckNode destroyed");
        delete(theChuck);
        theChuck = nullptr;
    }

    /// <summary>
    /// Assign reference to inlet params
    /// </summary>
    /// <returns></returns>
    FDataReferenceCollection FChuckOperator::GetInputs() const
    {
        using namespace ChuckNode;

        FDataReferenceCollection InputDataReferences;

        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameTrigger), Trigger);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameCode), Code);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), AudioInput);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameAmplitude), Amplitude);

        return InputDataReferences;
    }

    /// <summary>
    /// Assign reference to outlet param
    /// </summary>
    /// <returns></returns>
    FDataReferenceCollection FChuckOperator::GetOutputs() const
    {
        using namespace ChuckNode;

        FDataReferenceCollection OutputDataReferences;

        OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutParamNameAudio), AudioOutput);

        return OutputDataReferences;
    }

    /// <summary>
    /// Process audio block
    /// </summary>
    void FChuckOperator::Execute()
    {
        const FTrigger& trigger = *Trigger;
        const float* inbuffer = AudioInput->GetData();
        float* outbuffer      = AudioOutput->GetData();
        const int32 numSamples = AudioInput->Num();

        //Check Trigger & Run Chuck Code
        if (trigger.IsTriggered())
        {
            theChuck->compileCode(TCHAR_TO_UTF8(**Code), "", 1);
        }
        
        //Process
        theChuck->run((float*)inbuffer, outbuffer, numSamples);

        
    }

    /// <summary>
    /// Declare params
    /// </summary>
    /// <returns></returns>
    const FVertexInterface& FChuckOperator::GetVertexInterface()
    {
        using namespace ChuckNode;

        static const FVertexInterface Interface(
            FInputVertexInterface(
                TInputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameTrigger)),
                TInputDataVertex<FString>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameCode), FString(" ")),
                TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInput)),
                TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAmplitude), 1.0f)
            ),

            FOutputVertexInterface(
                TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudio))
            )
        );

        return Interface;
    }

    /// <summary>
    /// Define node info
    /// </summary>
    /// <returns></returns>
    const FNodeClassMetadata& FChuckOperator::GetNodeInfo()
    {
        auto InitNodeInfo = []() -> FNodeClassMetadata
        {
            FNodeClassMetadata Info;

            Info.ClassName        = { TEXT("UE"), TEXT("Chuck"), TEXT("Audio") };
            Info.MajorVersion     = 1;
            Info.MinorVersion     = 0;
            Info.DisplayName      = LOCTEXT("ChuckDisplayName", "Chuck");
            Info.Description      = LOCTEXT("ChuckNodeDescription", "This is The Chuck Node!");
            Info.Author           = PluginAuthor;
            Info.PromptIfMissing  = PluginNodeMissingPrompt;
            Info.DefaultInterface = GetVertexInterface();
            Info.CategoryHierarchy = { LOCTEXT("ChuckNodeCategory", "Utils") };

            return Info;
        };

        static const FNodeClassMetadata Info = InitNodeInfo();

        return Info;
    }

    /// <summary>
    /// Create operator
    /// </summary>
    /// <param name="InParams"></param>
    /// <param name="OutErrors"></param>
    /// <returns></returns>
    TUniquePtr<IOperator> FChuckOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
    {
        using namespace ChuckNode;

        const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
        const FInputVertexInterface& InputInterface     = GetVertexInterface().GetInputInterface();

        FTriggerReadRef InTrigger = InputCollection.GetDataReadReferenceOrConstruct<FTrigger>(METASOUND_GET_PARAM_NAME(InParamNameTrigger), InParams.OperatorSettings);
        FStringReadRef InCode = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FString>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameCode), InParams.OperatorSettings);
        FAudioBufferReadRef AudioIn = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInput), InParams.OperatorSettings);
        FFloatReadRef InAmplitude   = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameAmplitude), InParams.OperatorSettings);

        return MakeUnique<FChuckOperator>(InParams.OperatorSettings, InTrigger, InCode, AudioIn, InAmplitude);
    }


    //------------------------------------------------------------------------------------
    // FChuckNode
    //------------------------------------------------------------------------------------
    FChuckNode::FChuckNode(const FNodeInitData& InitData)
        : FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FChuckOperator>())
    {
    }

    METASOUND_REGISTER_NODE(FChuckNode)
}

#undef LOCTEXT_NAMESPACE
