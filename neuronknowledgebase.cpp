#include "neuronknowledgebase.h"



NeuronKnowledgeBase::NeuronKnowledgeBase()
{
    QVector<int> inputgrade = { 0, 0, 0 };
    evaluationSystem = new EvaluationSystem(true,inputgrade);
}

NeuronKnowledgeBase::NeuronKnowledgeBase(QVector<QVector<int>> input_combinations, bool positivegrade, QVector<int> inputgrade, int number_of_action)
{
    evaluationSystem = new EvaluationSystem(positivegrade,inputgrade);

    QPair<QVector<int>,int> temp;

    //save for next usage
    basic_input_combinations = input_combinations;


    for(int i = 0; i < input_combinations.size(); i++)
    {
        temp.first = input_combinations[i];
        temp.second = i;
        sortend_input_pattern.append(temp);
        gradeTable.append(evaluationSystem->gradePattern(input_combinations[i]));

    }

    //need sort
    doubleSort(gradeTable, sortend_input_pattern);

    for(int i = 0; i < input_combinations.size() * input_combinations.size() * number_of_action; i++)
    {
       Patterns.append(std::shared_ptr<neuron_AAC_type_1>(new neuron_AAC_type_1("neuron_type_1", QString::number(i),"0", 3, 10, 0.8, 1.0, 0)));
    }
    actions = number_of_action;



}

//---------------------------------------------------------------------------------------------------
void NeuronKnowledgeBase::doubleSort(QVector<int> &sorted_mass,  QVector<QPair<QVector<int>, int> > &connected_sorted_mass)  //void insertSort(int *a, int n)
{
    int j=0;
    for(int i = 1; i < sorted_mass.size(); i++)
    {
        j = i;
        while (j > 0 && sorted_mass[j] > sorted_mass[j-1])
        {
            std::swap(sorted_mass[j], sorted_mass[j-1]);
            std::swap(connected_sorted_mass[j], connected_sorted_mass[j-1]);
            j=j-1;
        }
    }
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

int NeuronKnowledgeBase::run(QVector<int> FRO_vector_run_previous)
{
    int action_answer = -1;
    //first we need to save previous result
    //accept current pattern and action and try to get result


    //next we need to offer new action


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

