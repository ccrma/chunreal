//-----------------------------------------------------------------------------
// file: ChuckMainNode.cpp
// desc: Chuck MetaSound main instance definition.
//
// Template MetaSound code provided by Epic Games.
// 
// authors: Eito Murakami (https://ccrma.stanford.edu/~eitom/) and Ge Wang (https://ccrma.stanford.edu/~ge/)
// date: Spring 2023
//-----------------------------------------------------------------------------

#include "ChuckMainNode.h"

#define LOCTEXT_NAMESPACE "Metasound_ChuckMainNode"

namespace Metasound
{
    //------------------------------------------------------------------------------------
    // FChuckMainOperator
    //------------------------------------------------------------------------------------
    FChuckMainOperator::FChuckMainOperator(const FOperatorSettings& InSettings, const FTriggerReadRef& InTrigger, const FStringReadRef& InCode, const FStringReadRef& InID, const FAudioBufferReadRef& InAudioInputLeft, const FAudioBufferReadRef& InAudioInputRight, const FFloatReadRef& InAmplitude)
        : Trigger(InTrigger)
        , Code(InCode)
        , ID(InID)
        , AudioInputLeft(InAudioInputLeft)
        , AudioInputRight(InAudioInputRight)
        , Amplitude(InAmplitude)
        , AudioOutputLeft(FAudioBufferWriteRef::CreateNew(InSettings))
        , AudioOutputRight(FAudioBufferWriteRef::CreateNew(InSettings))
        
    { 
        //Create Chuck
        theChuck = new ChucK();

        //Initialize Chuck params
        theChuck->setParam(CHUCK_PARAM_SAMPLE_RATE, FChunrealModule::GetChuckSampleRate());
        theChuck->setParam(CHUCK_PARAM_INPUT_CHANNELS, 2);
        theChuck->setParam(CHUCK_PARAM_OUTPUT_CHANNELS, 2);
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

        //Set log
        #if PRINT_CHUCK_LOG
        theChuck->setStdoutCallback(FChunrealModule::printThisFromChuck);
        theChuck->setStderrCallback(FChunrealModule::printThisFromChuck);
        //theChuck->setLogLevel(CK_LOG_INFO);
        #endif

        //Start Chuck
        theChuck->init();
        theChuck->start();

        //Store ChucK reference with ID
        if (!((FString)(**ID)).IsEmpty())
        {
            FChunrealModule::StoreChuckRef(theChuck, **ID);
            //FChunrealModule::Log(FString("Added ChucK ID: ") + **ID);
        }
    }
    FChuckMainOperator::~FChuckMainOperator()
    {
        //Remove ChucK reference with ID
        if (!((FString)(**ID)).IsEmpty())
        {
            FChunrealModule::RemoveChuckRef(**ID);
            //FChunrealModule::Log(FString("Removed ChucK ID: ") + **ID);
        }

        //Delete allocated memory
        delete inBufferInterleaved;
        delete outBufferInterleaved;

        //Delete ChucK
        delete theChuck;
        theChuck = nullptr;
    }

    /// <summary>
    /// Assign reference to inlet params
    /// </summary>
    /// <returns></returns>
    FDataReferenceCollection FChuckMainOperator::GetInputs() const
    {
        using namespace ChuckMainNode;

        FDataReferenceCollection InputDataReferences;

        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameTrigger), Trigger);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameCode), Code);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameID), ID);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameAudioInputLeft), AudioInputLeft);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameAudioInputRight), AudioInputRight);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameAmplitude), Amplitude);

        return InputDataReferences;
    }

    /// <summary>
    /// Assign reference to outlet param
    /// </summary>
    /// <returns></returns>
    FDataReferenceCollection FChuckMainOperator::GetOutputs() const
    {
        using namespace ChuckMainNode;

        FDataReferenceCollection OutputDataReferences;

        OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutParamNameAudioOutputLeft), AudioOutputLeft);
        OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutParamNameAudioOutputRight), AudioOutputRight);

        return OutputDataReferences;
    }

    /// <summary>
    /// Process audio block
    /// </summary>
    void FChuckMainOperator::Execute()
    {
        const FTrigger& trigger = *Trigger;
        const float* inBufferLeft = AudioInputLeft->GetData();
        const float* inBufferRight = AudioInputRight->GetData();
        float* outBufferLeft = AudioOutputLeft->GetData();
        float* outBufferRight = AudioOutputRight->GetData();
        const int32 numSamples = AudioInputLeft->Num();

        //Run ChucK code
        if (trigger.IsTriggered())
        {
            if (hasSporkedOnce)
            {
                Chuck_Msg* msg = new Chuck_Msg;
                msg->type = 3;  //MSG_REMOVEALL
                theChuck->vm()->process_msg(msg);
            }
            else
            {
                hasSporkedOnce = true;
            }
            FChunrealModule::CompileChuckCode(theChuck, TCHAR_TO_UTF8(**Code));
        }

        //Make interleaved buffers
        if (!bufferInitialized)
        {
            inBufferInterleaved = new float[numSamples * 2];
            outBufferInterleaved = new float[numSamples * 2];

            bufferInitialized = true;
        }
        for (int i = 0; i < numSamples; i++)
        {
            *(inBufferInterleaved + i * 2) = *(inBufferLeft + i);
            *(inBufferInterleaved + i * 2 + 1) = *(inBufferRight + i);
        }

        //Process samples by ChucK
        FChunrealModule::RunChuck(theChuck, (float*)inBufferInterleaved, outBufferInterleaved, numSamples);

        //Retrive each output channel and apply volume multiplier
        for (int i = 0; i < numSamples; i++)
        {
            *(outBufferLeft + i) = *(outBufferInterleaved + i * 2) * (*Amplitude);
            *(outBufferRight + i) = *(outBufferInterleaved + i * 2 + 1) * (*Amplitude);
        }
    }

    /// <summary>
    /// Declare params
    /// </summary>
    /// <returns></returns>
    const FVertexInterface& FChuckMainOperator::GetVertexInterface()
    {
        using namespace ChuckMainNode;

        static const FVertexInterface Interface(
            FInputVertexInterface(
                TInputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameTrigger)),
                TInputDataVertex<FString>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameCode), FString("")),
                TInputDataVertex<FString>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameID), FString("")),
                TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInputLeft)),
                TInputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAudioInputRight)),
                TInputDataVertex<float>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameAmplitude), 1.0f)
            ),

            FOutputVertexInterface(
                TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudioOutputLeft)),
                TOutputDataVertex<FAudioBuffer>(METASOUND_GET_PARAM_NAME_AND_METADATA(OutParamNameAudioOutputRight))
            )
        );

        return Interface;
    }

    /// <summary>
    /// Define node info
    /// </summary>
    /// <returns></returns>
    const FNodeClassMetadata& FChuckMainOperator::GetNodeInfo()
    {
        auto InitNodeInfo = []() -> FNodeClassMetadata
        {
            FNodeClassMetadata Info;

            Info.ClassName        = { TEXT("UE"), TEXT("ChuckMain"), TEXT("Audio") };
            Info.MajorVersion     = 1;
            Info.MinorVersion     = 0;
            Info.DisplayName      = LOCTEXT("ChuckDisplayName", "ChuckMain");
            Info.Description      = LOCTEXT("ChuckNodeDescription", "This is The ChucK Main Instance Node!");
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
    TUniquePtr<IOperator> FChuckMainOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
    {
        using namespace ChuckMainNode;

        const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
        const FInputVertexInterface& InputInterface     = GetVertexInterface().GetInputInterface();

        FTriggerReadRef InTrigger = InputCollection.GetDataReadReferenceOrConstruct<FTrigger>(METASOUND_GET_PARAM_NAME(InParamNameTrigger), InParams.OperatorSettings);
        FStringReadRef InCode = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FString>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameCode), InParams.OperatorSettings);
        FStringReadRef InID = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FString>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameID), InParams.OperatorSettings);
        FAudioBufferReadRef InAudioInputLeft = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInputLeft), InParams.OperatorSettings);
        FAudioBufferReadRef InAudioInputRight = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInputRight), InParams.OperatorSettings);
        FFloatReadRef InAmplitude   = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameAmplitude), InParams.OperatorSettings);

        return MakeUnique<FChuckMainOperator>(InParams.OperatorSettings, InTrigger, InCode, InID, InAudioInputLeft, InAudioInputRight, InAmplitude);
    }


    //------------------------------------------------------------------------------------
    // FChuckMainNode
    //------------------------------------------------------------------------------------
    FChuckMainNode::FChuckMainNode(const FNodeInitData& InitData)
        : FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FChuckMainOperator>())
    {
    }

    METASOUND_REGISTER_NODE(FChuckMainNode)
}

#undef LOCTEXT_NAMESPACE
