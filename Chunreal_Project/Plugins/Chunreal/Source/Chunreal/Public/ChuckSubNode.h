//-----------------------------------------------------------------------------
// file: ChuckSubNode.h
// desc: Chuck MetaSound sub instance header.
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
#define LOCTEXT_NAMESPACE "Metasound_ChuckSubNode"

    namespace ChuckSubNode
    {
        METASOUND_PARAM(InParamNameTrigger, "Run Code", "Trigger to run Chuck code.")
        METASOUND_PARAM(InParamNameCode, "Code", "Chuck code to run.")
    }

#undef LOCTEXT_NAMESPACE


    //------------------------------------------------------------------------------------
    // FChuckSubOperator
    //------------------------------------------------------------------------------------
    class FChuckSubOperator : public TExecutableOperator<FChuckSubOperator>
    {
    public:
        static const FNodeClassMetadata& GetNodeInfo();
        static const FVertexInterface& GetVertexInterface();
        static TUniquePtr<IOperator> CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors);

        FChuckSubOperator(const FOperatorSettings& InSettings, const FTriggerReadRef& InTrigger, const FStringReadRef& InCode);
        ~FChuckSubOperator();

        virtual FDataReferenceCollection GetInputs()  const override;
        virtual FDataReferenceCollection GetOutputs() const override;

        void Execute();

    private:
        //local variables
        FTriggerReadRef Trigger;
        FStringReadRef Code;

        bool hasSporkedOnce = false;

        std::vector<t_CKUINT> shredIDs;
    };

    //------------------------------------------------------------------------------------
    // FChuckSubNode
    //------------------------------------------------------------------------------------
    class FChuckSubNode : public FNodeFacade
    {
    public:
        // Constructor used by the Metasound Frontend.
        FChuckSubNode(const FNodeInitData& InitData);
    };
}
