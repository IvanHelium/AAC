#include "neuronknowledgebase.h"



NeuronKnowledgeBase::NeuronKnowledgeBase()
{
    QVector<int> inputgrade = { 0, 0, 0 };
    evaluationSystem = new EvaluationSystem(true,inputgrade);
}

NeuronKnowledgeBase::NeuronKnowledgeBase(bool positivegrade, QVector<int> inputgrade)
{
evaluationSystem = new EvaluationSystem(positivegrade,inputgrade);
}


//-------------------------------------------------------------------------------------------------

void NeuronKnowledgeBase::setKnowledgeBasePatternCurrent(QVector<int> knowledgeBasePatternCurrentInput)
{
knowledgeBasePatternCurrent = knowledgeBasePatternCurrentInput;
}

//-------------------------------------------------------------------------------------------------

void NeuronKnowledgeBase::setKnowledgeBasePatternPrevious(QVector<int> knowledgeBasePatternPreviousInput)
{
knowledgeBasePatternPrevious = knowledgeBasePatternPreviousInput;
}

//-------------------------------------------------------------------------------------------------

QVector<int> NeuronKnowledgeBase::getKnowledgeBasePatternCurrent()
{
 return knowledgeBasePatternCurrent;
}

//-------------------------------------------------------------------------------------------------

QVector<int> NeuronKnowledgeBase::getKnowledgeBasePatternPrevious()
{
return knowledgeBasePatternPrevious;
}

//-------------------------------------------------------------------------------------------------

void NeuronKnowledgeBase::setLastActionIndex(int index)
{
    lastActionIndex = index;
}

//-------------------------------------------------------------------------------------------------
int NeuronKnowledgeBase::getLastActionIndex()
{
 return lastActionIndex;
}


//-------------------------------------------------------------------------------------------------




//-----------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------
EvaluationSystem::EvaluationSystem()
{
positiveGrade = true;
}

EvaluationSystem::EvaluationSystem(bool positivegrade)
{
positiveGrade = positivegrade;
}

EvaluationSystem::EvaluationSystem(bool positivegrade, QVector<int> inputgrade)
{
positiveGrade = positivegrade;
inputGrade = inputgrade;
}



void EvaluationSystem::setInputGrade(QVector<int> inputgrade)
{
 inputGrade = inputgrade;
}

double EvaluationSystem::gradePattern(QVector<int> map)
{
    double result=0;

    int ones_count = 0;

    if(map.size()!=inputGrade.size())
    {
        return -1.0;
    }

    for(int i = 0; i < map.size(); i++)
    {
        if(map.at(i) == 1)
        {
            result += inputGrade.at(i);
            ones_count++;
        }
    }
    if(ones_count > 0)
    {
      result = result  / ones_count;
    }

    return result;
}

