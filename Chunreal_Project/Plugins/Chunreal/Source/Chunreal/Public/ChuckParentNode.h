//-----------------------------------------------------------------------------
// file: ChuckParentNode.h
// desc: Chuck MetaSound parent instance header.
//
// Template MetaSound code provided by Epic Games.
// 
// authors: Eito Murakami (https://ccrma.stanford.edu/~eitom/) and Ge Wang (https://ccrma.stanford.edu/~ge/)
// date: Fall 2023
//-----------------------------------------------------------------------------

#include "Chunreal.h"

#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
#define LOCTEXT_NAMESPACE "Metasound_ChuckParentNode"

    namespace ChuckParentNode
    {
        METASOUND_PARAM(InParamNameAudioInputLeft, "Audio Input Left", "Audio input left.")
        METASOUND_PARAM(InParamNameAudioInputRight, "Audio Input Right", "Audio input right.")
        METASOUND_PARAM(InParamNameAmplitude, "Volume Multiplier", "Volume multiplier.")
        METASOUND_PARAM(OutParamNameAudioOutputLeft, "Audio Output Left", "Audio output left.")
        METASOUND_PARAM(OutParamNameAudioOutputRight, "Audio Output Right", "Audio output right.")
    }

#undef LOCTEXT_NAMESPACE


    //------------------------------------------------------------------------------------
    // FChuckParentOperator
    //------------------------------------------------------------------------------------
    class FChuckParentOperator : public TExecutableOperator<FChuckParentOperator>
    {
    public:
        static const FNodeClassMetadata& GetNodeInfo();
        static const FVertexInterface& GetVertexInterface();
        static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

        FChuckParentOperator(const FOperatorSettings& InSettings, const FAudioBufferReadRef& InAudioInputLeft, const FAudioBufferReadRef& InAudioInputRight, const FFloatReadRef& InAmplitude);
        ~FChuckParentOperator();

        virtual FDataReferenceCollection GetInputs()  const override;
        virtual FDataReferenceCollection GetOutputs() const override;

        void Execute();

    private:
        //audio input
        FAudioBufferReadRef  AudioInputLeft;
        FAudioBufferReadRef  AudioInputRight;

        //amplitude
        FFloatReadRef Amplitude;

        //audio output
        FAudioBufferWriteRef AudioOutputLeft;
        FAudioBufferWriteRef AudioOutputRight;

        //interleaved buffers
        float* inBufferInterleaved;
        float* outBufferInterleaved;

        bool bufferInitialized = false;
    };

    //------------------------------------------------------------------------------------
    // FChuckParentNode
    //------------------------------------------------------------------------------------
    class FChuckParentNode : public FNodeFacade
    {
    public:
        // Constructor used by the Metasound Frontend.
        FChuckParentNode(const FNodeInitData& InitData);
    };
}
