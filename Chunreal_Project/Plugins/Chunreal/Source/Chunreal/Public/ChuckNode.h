//-----------------------------------------------------------------------------
// file: ChuckNode.h
// desc: Chuck MetaSound node header.
//
// Template MetaSound code provided by Epic Games.
// 
// authors: Eito Murakami (https://ccrma.stanford.edu/~eitom/) and Ge Wang (https://ccrma.stanford.edu/~ge/)
// date: Spring 2023
//-----------------------------------------------------------------------------

#include "Chunreal.h"
class ChucK;

#include "MetasoundEnumRegistrationMacro.h"
#include "MetasoundParamHelper.h"

namespace Metasound
{
#define LOCTEXT_NAMESPACE "Metasound_ChuckNode"

    namespace ChuckNode
    {
        METASOUND_PARAM(InParamNameTrigger, "RunCode", "Trigger to run Chuck code.")
        METASOUND_PARAM(InParamNameCode, "Code", "Chuck code to run.")
        METASOUND_PARAM(InParamNameAudioInput, "AudioInput",        "Audio input.")
        METASOUND_PARAM(InParamNameAmplitude,  "VolumeMultiplier", "Volume multiplier.")
        METASOUND_PARAM(OutParamNameAudio,     "Out",       "Audio output.")
    }

#undef LOCTEXT_NAMESPACE


    //------------------------------------------------------------------------------------
    // FChuckOperator
    //------------------------------------------------------------------------------------
    class FChuckOperator : public TExecutableOperator<FChuckOperator>
    {
    public:
        static const FNodeClassMetadata& GetNodeInfo();
        static const FVertexInterface& GetVertexInterface();
        static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

        FChuckOperator(const FOperatorSettings& InSettings, const FTriggerReadRef& InTrigger, const FStringReadRef& InCode, const FAudioBufferReadRef& InAudioInput, const FFloatReadRef& InAmplitude);
        ~FChuckOperator();

        virtual FDataReferenceCollection GetInputs()  const override;
        virtual FDataReferenceCollection GetOutputs() const override;

        void Execute();

    private:
        FAudioBufferReadRef  AudioInput;
        FAudioBufferWriteRef AudioOutput;

        ChucK* theChuck;
        FTriggerReadRef Trigger;
        FStringReadRef Code;
        FFloatReadRef Amplitude;
    };

    //------------------------------------------------------------------------------------
    // FChuckNode
    //------------------------------------------------------------------------------------
    class FChuckNode : public FNodeFacade
    {
    public:
        // Constructor used by the Metasound Frontend.
        FChuckNode(const FNodeInitData& InitData);
    };
}
