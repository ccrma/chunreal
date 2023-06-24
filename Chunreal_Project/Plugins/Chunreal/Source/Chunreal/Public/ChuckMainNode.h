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
        METASOUND_PARAM(InParamNameTrigger, "RunCode", "Trigger to run Chuck code.")
        METASOUND_PARAM(InParamNameCode, "Code", "Chuck code to run.")
        METASOUND_PARAM(InParamNameID, "ID", "Unique ID assigned to ChucK")
        METASOUND_PARAM(InParamNameAudioInput, "AudioInput",        "Audio input.")
        METASOUND_PARAM(InParamNameAmplitude,  "VolumeMultiplier", "Volume multiplier.")
        METASOUND_PARAM(OutParamNameAudio,     "Out",       "Audio output.")
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

        FChuckMainOperator(const FOperatorSettings& InSettings, const FTriggerReadRef& InTrigger, const FStringReadRef& InCode, const FStringReadRef& InID, const FAudioBufferReadRef& InAudioInput, const FFloatReadRef& InAmplitude);
        ~FChuckMainOperator();

        virtual FDataReferenceCollection GetInputs()  const override;
        virtual FDataReferenceCollection GetOutputs() const override;

        void Execute();

    private:
        FAudioBufferReadRef  AudioInput;
        FAudioBufferWriteRef AudioOutput;

        FTriggerReadRef Trigger;
        FStringReadRef Code;
        FStringReadRef ID;
        FFloatReadRef Amplitude;

        //reference to chuck
        ChucK* theChuck = nullptr;

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
