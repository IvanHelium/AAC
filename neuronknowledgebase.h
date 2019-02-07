#ifndef NEURONKNOWLEDGEBASE_H
#define NEURONKNOWLEDGEBASE_H

#include "neuron_aac.h"
#include <QTimer>
#include <QPair>

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>

#include <cstdlib>
#include <iostream>
#include <ctime>

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
    NeuronKnowledgeBase(QVector<QVector<int>> input_combinations, bool positivegrade, QVector<int> inputgrade, int number_of_action, int unique_pattern_window_iterration);

    void setKnowledgeBasePatternCurrent(QVector<int> knowledgeBasePatternCurrentInput);
    QVector<int> getKnowledgeBasePatternCurrent();
    void setKnowledgeBasePatternPrevious(QVector<int> knowledgeBasePatternPreviousInput);
    QVector<int> getKnowledgeBasePatternPrevious();

    void setLastActionIndex(int index);
    int getLastActionIndex();

    void doubleSort(QVector<int> &sorted_mass,  QVector<QPair<QVector<int>, int> > &connected_sorted_mass);

    int run();

    QString get_debug_text();

    void save_KnowledgeBase_to_file(QString path);

    void load_knowladgebase_from_file(QString path);

    double probability_of_choosing_a_random_action(int max_length, int current_index); //function has a certain form



private:
    QList<std::shared_ptr<neuron_AAC_type_1>> Patterns;
    int actions;//de
    int L; //variable indicate neuron threshold , used for counting actions

    EvaluationSystem *evaluationSystem;

    QVector<int> knowledgeBasePatternCurrent;
    QVector<int> knowledgeBasePatternPrevious;

    int lastActionIndex;


    QVector<QPair<QVector<int>,int>> sortend_input_pattern;

    QVector<int> gradeTable;

    QVector<QVector<int>> basic_input_combinations;

    QString debug_text;

};







#endif // NEURONKNOWLEDGEBASE_H
