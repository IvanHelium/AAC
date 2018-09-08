#ifndef NEURONKNOWLEDGEBASE_H
#define NEURONKNOWLEDGEBASE_H

#include "neuron_aac.h"
#include <QTimer>


class EvaluationSystem
{
public:
    EvaluationSystem();
    EvaluationSystem(bool positivegrade);
    EvaluationSystem(bool positivegrade, QVector<int> inputgrade);
    void setInputGrade(QVector<int> inputgrade);
    double gradePattern(QVector<int> map);

private:
    bool positiveGrade;     //if true maximum value result is better result, if not - minimum
    QVector<int> inputGrade;



};


class NeuronKnowledgeBase
{
public:
    NeuronKnowledgeBase();
    NeuronKnowledgeBase(bool positivegrade, QVector<int> inputgrade);

    void setKnowledgeBasePatternCurrent(QVector<int> knowledgeBasePatternCurrentInput);
    QVector<int> getKnowledgeBasePatternCurrent();
    void setKnowledgeBasePatternPrevious(QVector<int> knowledgeBasePatternPreviousInput);
    QVector<int> getKnowledgeBasePatternPrevious();

    void setLastActionIndex(int index);
    int getLastActionIndex();



private:
    QList<std::shared_ptr<neuron_AAC_type_1>> Patterns; //de
    QList<std::shared_ptr<neuron_AAC_type_1>> sortedPatterns;//de
    QList<int> actions;//de
    int L; //variable indicate neuron threshold , used for counting actions

    EvaluationSystem *evaluationSystem;

    QVector<int> knowledgeBasePatternCurrent;
    QVector<int> knowledgeBasePatternPrevious;

    int lastActionIndex;
};







#endif // NEURONKNOWLEDGEBASE_H
