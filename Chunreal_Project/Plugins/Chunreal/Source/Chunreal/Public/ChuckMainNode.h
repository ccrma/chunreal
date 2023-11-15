//-----------------------------------------------------------------------------
// file: ChuckMainNode.h
// desc: Chuck MetaSound main instance header.
//
// Template MetaSound code provided by Epic Games.
// 
// authors: Eito Murakami (https://ccrma.stanford.edu/~eitom/) and Ge Wang (https://ccrma.stanford.edu/~ge/)
// date: Spring 2023
//-----------------------------------------------------------------------------

#include "Chunreal.h"

#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"
#include "Chunreal/chuck/chuck.h"
#include "Chunreal/chuck/chuck_def.h"

namespace Metasound
{
#define LOCTEXT_NAMESPACE "Metasound_ChuckMainNode"

    namespace ChuckMainNode
    {
        METASOUND_PARAM(InParamNameTrigger, "Run Code", "Trigger to run Chuck code.")
        METASOUND_PARAM(InParamNameCode, "Code", "Chuck code to run.")
        METASOUND_PARAM(InParamNameID, "ID", "Unique ID assigned to ChucK")
        METASOUND_PARAM(InParamNameAudioInputLeft, "Audio Input Left", "Audio input left.")
        METASOUND_PARAM(InParamNameAudioInputRight, "Audio Input Right", "Audio input right.")
        METASOUND_PARAM(InParamNameAmplitude, "Volume Multiplier", "Volume multiplier.")
        METASOUND_PARAM(OutParamNameAudioOutputLeft, "Audio Output Left", "Audio output left.")
        METASOUND_PARAM(OutParamNameAudioOutputRight, "Audio Output Right", "Audio output right.")
    }

#undef LOCTEXT_NAMESPACE


    //------------------------------------------------------------------------------------
    // FChuckMainOperator
    //------------------------------------------------------------------------------------
    class FChuckMainOperator : public TExecutableOperator<FChuckMainOperator>
    {
    public:
        static const FNodeClassMetadata& GetNodeInfo();
        static const FVertexInterface& GetVertexInterface();
        static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

        FChuckMainOperator(const FOperatorSettings& InSettings, const FTriggerReadRef& InTrigger, const FStringReadRef& InCode, const FStringReadRef& InID, const FAudioBufferReadRef& InAudioInputLeft, const FAudioBufferReadRef& InAudioInputRight, const FFloatReadRef& InAmplitude);
        ~FChuckMainOperator();

        virtual FDataReferenceCollection GetInputs()  const override;
        virtual FDataReferenceCollection GetOutputs() const override;

        void Execute();

    private:
        //local variables
        FTriggerReadRef Trigger;
        FStringReadRef Code;
        FStringReadRef ID;

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

        //reference to chuck
        ChucK* theChuck = nullptr;

        bool bufferInitialized = false;
        bool hasSporkedOnce = false;
    };

    //------------------------------------------------------------------------------------
    // FChuckMainNode
    //------------------------------------------------------------------------------------
    class FChuckMainNode : public FNodeFacade
    {
    public:
        // Constructor used by the Metasound Frontend.
        FChuckMainNode(const FNodeInitData& InitData);
    };
}
