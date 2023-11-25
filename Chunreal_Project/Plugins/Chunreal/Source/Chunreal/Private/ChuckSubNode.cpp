//-----------------------------------------------------------------------------
// file: ChuckSubNode.cpp
// desc: Chuck MetaSound sub instance definition.
//
// Template MetaSound code provided by Epic Games.
// 
// authors: Eito Murakami (https://ccrma.stanford.edu/~eitom/) and Ge Wang (https://ccrma.stanford.edu/~ge/)
// date: Fall 2023
//-----------------------------------------------------------------------------

#include "ChuckSubNode.h"

#define LOCTEXT_NAMESPACE "Metasound_ChuckSubNode"

namespace Metasound
{
    //------------------------------------------------------------------------------------
    // FChuckSubOperator
    //------------------------------------------------------------------------------------
    FChuckSubOperator::FChuckSubOperator(const FOperatorSettings& InSettings, const FTriggerReadRef& InTrigger, const FStringReadRef& InCode)
        : Trigger(InTrigger)
        , Code(InCode)
    {
    }
    FChuckSubOperator::~FChuckSubOperator()
    {
        //Remove sporked shread
        if (shredIDs.size() > 0)
        {
            FString removeShred = "Machine.remove(shredID); Machine.remove(me.id());";
            removeShred = removeShred.Replace(TEXT("shredID"), *FString::FromInt(shredIDs.at(0)));
            FChunrealModule::CompileChuckCode(FChunrealModule::chuckParent, TCHAR_TO_UTF8(*removeShred));
        }
    }

    /// <summary>
    /// Assign reference to inlet params
    /// </summary>
    /// <returns></returns>
    FDataReferenceCollection FChuckSubOperator::GetInputs() const
    {
        using namespace ChuckSubNode;

        FDataReferenceCollection InputDataReferences;

        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameTrigger), Trigger);
        InputDataReferences.AddDataReadReference(METASOUND_GET_PARAM_NAME(InParamNameCode), Code);

        return InputDataReferences;
    }

    /// <summary>
    /// Assign reference to outlet param
    /// </summary>
    /// <returns></returns>
    FDataReferenceCollection FChuckSubOperator::GetOutputs() const
    {
        using namespace ChuckSubNode;

        FDataReferenceCollection OutputDataReferences;

        return OutputDataReferences;
    }

    /// <summary>
    /// Process audio block
    /// </summary>
    void FChuckSubOperator::Execute()
    {
        const FTrigger& trigger = *Trigger;

        //Run ChucK Code
        if (trigger.IsTriggered())
        {
            if (hasSporkedOnce)
            {
                //Remove sporked shread
                if (shredIDs.size() > 0)
                {
                    FString removeShred = "Machine.remove(shredID); Machine.remove(me.id());";
                    removeShred = removeShred.Replace(TEXT("shredID"), *FString::FromInt(shredIDs.at(0)));
                    FChunrealModule::CompileChuckCode(FChunrealModule::chuckParent, TCHAR_TO_UTF8(*removeShred));
                }
            }
            else
            {
                hasSporkedOnce = true;
            }

            FChunrealModule::CompileChuckCode(FChunrealModule::chuckParent, TCHAR_TO_UTF8(**Code), &shredIDs);
        }
    }

    /// <summary>
    /// Declare params
    /// </summary>
    /// <returns></returns>
    const FVertexInterface& FChuckSubOperator::GetVertexInterface()
    {
        using namespace ChuckSubNode;

        static const FVertexInterface Interface(
            FInputVertexInterface(
                TInputDataVertex<FTrigger>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameTrigger)),
                TInputDataVertex<FString>(METASOUND_GET_PARAM_NAME_AND_METADATA(InParamNameCode), FString(""))
            ),

            FOutputVertexInterface(
            )
        );

        return Interface;
    }

    /// <summary>
    /// Define node info
    /// </summary>
    /// <returns></returns>
    const FNodeClassMetadata& FChuckSubOperator::GetNodeInfo()
    {
        auto InitNodeInfo = []() -> FNodeClassMetadata
        {
            FNodeClassMetadata Info;

            Info.ClassName        = { TEXT("UE"), TEXT("ChuckSub"), TEXT("Audio") };
            Info.MajorVersion     = 1;
            Info.MinorVersion     = 0;
            Info.DisplayName      = LOCTEXT("ChuckDisplayName", "ChuckSub");
            Info.Description      = LOCTEXT("ChuckNodeDescription", "This is The ChucK Sub Instance Node!");
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
    TUniquePtr<IOperator> FChuckSubOperator::CreateOperator(const FCreateOperatorParams& InParams, FBuildErrorArray& OutErrors)
    {
        using namespace ChuckSubNode;

        const FDataReferenceCollection& InputCollection = InParams.InputDataReferences;
        const FInputVertexInterface& InputInterface     = GetVertexInterface().GetInputInterface();

        FTriggerReadRef InTrigger = InputCollection.GetDataReadReferenceOrConstruct<FTrigger>(METASOUND_GET_PARAM_NAME(InParamNameTrigger), InParams.OperatorSettings);
        FStringReadRef InCode = InputCollection.GetDataReadReferenceOrConstructWithVertexDefault<FString>(InputInterface, METASOUND_GET_PARAM_NAME(InParamNameCode), InParams.OperatorSettings);

        return MakeUnique<FChuckSubOperator>(InParams.OperatorSettings, InTrigger, InCode);
    }


    //------------------------------------------------------------------------------------
    // FChuckSubNode
    //------------------------------------------------------------------------------------
    FChuckSubNode::FChuckSubNode(const FNodeInitData& InitData)
        : FNodeFacade(InitData.InstanceName, InitData.InstanceID, TFacadeOperatorClass<FChuckSubOperator>())
    {
    }

    METASOUND_REGISTER_NODE(FChuckSubNode)
}

#undef LOCTEXT_NAMESPACE
