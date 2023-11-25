//-----------------------------------------------------------------------------
// file: ChuckParentNode.cpp
// desc: Chuck MetaSound parent instance definition.
//
// Template MetaSound code provided by Epic Games.
// 
// authors: Eito Murakami (https://ccrma.stanford.edu/~eitom/) and Ge Wang (https://ccrma.stanford.edu/~ge/)
// date: Fall 2023
//-----------------------------------------------------------------------------

#include "ChuckParentNode.h"

#define LOCTEXT_NAMESPACE "Metasound_ChuckParentNode"

namespace Metasound
{
    //------------------------------------------------------------------------------------
    // FChuckParentOperator
    //------------------------------------------------------------------------------------
    FChuckParentOperator::FChuckParentOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInputLeft, const FAudioBufferReadRef& InAudioInputRight, const FFloatReadRef& InAmplitude)
        : AudioInputLeft(InAudioInputLeft)
        , AudioInputRight(InAudioInputRight)
        , Amplitude(InAmplitude)
        , AudioOutputLeft(FAudioBufferWriteRef::CreateNew(InSettings))
        , AudioOutputRight(FAudioBufferWriteRef::CreateNew(InSettings))
    { 
    }
    FChuckParentOperator::~FChuckParentOperator()
    {
        //Delete allocated memory
        delete inBufferInterleaved;
        delete outBufferInterleaved;
    }

    /// <summary>
    /// Assign reference to inlet params
    /// </summary>
    /// <returns></returns>
    FDataReferenceCollection FChuckParentOperator::GetInputs() const
    {
        using namespace ChuckParentNode;

        FDataReferenceCollection InputDataReferences;

        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameAudioInputLeft), AudioInputLeft);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameAudioInputRight), AudioInputRight);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameAmplitude), Amplitude);

        return InputDataReferences;
    }

    /// <summary>
    /// Assign reference to outlet param
    /// </summary>
    /// <returns></returns>
    FDataReferenceCollection FChuckParentOperator::GetOutputs() const
    {
        using namespace ChuckParentNode;

        FDataReferenceCollection OutputDataReferences;

        OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutParamNameAudioOutputLeft), AudioOutputLeft);
        OutputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(OutParamNameAudioOutputRight), AudioOutputRight);

        return OutputDataReferences;
    }

    /// <summary>
    /// Process audio block
    /// </summary>
    void FChuckParentOperator::Execute()
    {
        const float* inBufferLeft = AudioInputLeft->GetData();
        const float* inBufferRight = AudioInputRight->GetData();
        float* outBufferLeft = AudioOutputLeft->GetData();
        float* outBufferRight = AudioOutputRight->GetData();
        const int32 numSamples = AudioInputLeft->Num();
       
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
        FChunrealModule::RunChuck(FChunrealModule::chuckParent, (float*)inBufferInterleaved, outBufferInterleaved, numSamples);

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
    const FVertexInterface& FChuckParentOperator::GetVertexInterface()
    {
        using namespace ChuckParentNode;

        static const FVertexInterface Interface(
            FInputVertexInterface(
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
    const FNodeClassMetadata& FChuckParentOperator::GetNodeInfo()
    {
        auto InitNodeInfo = []() -> FNodeClassMetadata
        {
            FNodeClassMetadata Info;

            Info.ClassName        = { TEXT("UE"), TEXT("ChuckParent"), TEXT("Audio") };
            Info.MajorVersion     = 1;
            Info.MinorVersion     = 0;
            Info.DisplayName      = LOCTEXT("ChuckDisplayName", "ChuckParent");
            Info.Description      = LOCTEXT("ChuckNodeDescription", "This is The ChucK Parent Node!");
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
    TUniquePtr<IOperator> FChuckParentOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
    {
        using namespace ChuckParentNode;

        const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
        const FInputVertexInterface& InputInterface     = GetVertexInterface().GetInputInterface();

        FAudioBufferReadRef InAudioInputLeft = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInputLeft), InParams.OperatorSettings);
        FAudioBufferReadRef InAudioInputRight = InputCollection.GetDataReadReferenceOrConstruct<FAudioBuffer>(METASOUND_GET_PARAM_NAME(InParamNameAudioInputRight), InParams.OperatorSettings);
        FFloatReadRef InAmplitude = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<float>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameAmplitude), InParams.OperatorSettings);

        return MakeUnique<FChuckParentOperator>(InParams.OperatorSettings, InAudioInputLeft, InAudioInputRight, InAmplitude);
    }


    //------------------------------------------------------------------------------------
    // FChuckParentNode
    //------------------------------------------------------------------------------------
    FChuckParentNode::FChuckParentNode(const FNodeInitData& InitData)
        : FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FChuckParentOperator>())
    {
    }

    METASOUND_REGISTER_NODE(FChuckParentNode)
}

#undef LOCTEXT_NAMESPACE
