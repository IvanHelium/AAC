#include "neuronknowledgebase.h"

#include <QTime>
#include <QDateTime>

#include <QDebug>
#include <QtGlobal>
#include <QTextCodec>
#include <math.h>

#define MAX_ACTIONS 6

NeuronKnowledgeBase::NeuronKnowledgeBase()
{
    srand(time(nullptr));
    QVector<int> inputgrade = { 0, 0, 0 };
    evaluationSystem = new EvaluationSystem(true,inputgrade);

}

NeuronKnowledgeBase::NeuronKnowledgeBase(QVector<QVector<int>> input_combinations, bool positivegrade, QVector<int> inputgrade, int number_of_action, int unique_pattern_window_iterration)
{
    srand(time(nullptr));
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

    for(int i = 0; i < (input_combinations.size()) * (input_combinations.size()) * (number_of_action); i++)
    {
       Patterns.append(std::shared_ptr<neuron_AAC_type_1>(new neuron_AAC_type_1("neuron_type_1", QString::number(i),"0", 3, 2, 0.6, 1.0, 0)));
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
 if(lastActionIndex < 0)
 {
     lastActionIndex = 0;
 }
 return lastActionIndex;
}


//-------------------------------------------------------------------------------------------------

int NeuronKnowledgeBase::run()
{
    QDateTime start = QDateTime::currentDateTime();



    int count = 0;
    volatile int out_temp =0;
    int action_answer = -1;

    volatile double filled_layer= 0.00000;

    int basic_input_combinations_index = -1;

    QVector<int> pattern_input_to_knowladgebase_offer = {1, 0, 1};
    QVector<int> pattern_full_save = {1, 1, 1};

    int i1 = 0; //to knowladgebase neuron index
    int j1 = 0; //to knowladgebase neuron index
    int k1 = 0; //to knowladgebase neuron index

    for(int i = 0; i < basic_input_combinations.size(); i++) //finding index of current pattern
    {
        if(knowledgeBasePatternCurrent == basic_input_combinations.at(i))
        {
            basic_input_combinations_index = i;
            break;
        }
    }


       // qDebug() << QString::number(basic_input_combinations_index) << endl;





    j1 = lastActionIndex;
    if(j1 < 0 || j1 > MAX_ACTIONS - 1)
    {
        j1=0;
    }

    for(int i = 0; i < sortend_input_pattern.size(); i++) //apply index to k1 index for database
    {
        if(sortend_input_pattern.at(i).second == basic_input_combinations_index)
        {
            k1 = i;  //current pattern = previous result puttern
        }
    }


    for(int i = 0; i < basic_input_combinations.size(); i++) //finding index of previous  pattern
    {
        if(knowledgeBasePatternPrevious == basic_input_combinations.at(i))
        {
            basic_input_combinations_index = i;
            break;
        }
    }

    for(int i = 0; i < sortend_input_pattern.size(); i++) //apply index to i1 index for database
    {
        if(sortend_input_pattern.at(i).second == basic_input_combinations_index)
        {
            i1 = i;  //current pattern = previous result puttern
        }
    }

    //now we have all 3 index to save to database


     if ((i1+1 * j1+1 * k1+1) > 48600 -1)
     {
      //  qDebug() << " out of  f index  = " + QString::number((i1+1) * (j1+1) * (k1+1)) << endl <<  " i1 =" +QString::number(i1) + " j1 " + QString::number(j1) + " k1 =" +QString::number(k1);
     }
    Patterns.at((i1*6*90) + (j1*90) + (k1))->run(pattern_full_save);
    Patterns.at((i1*6*90) + (j1*90) + (k1))->sync();
    Patterns.at((i1*6*90) + (j1*90) + (k1))->reset(); //reset because it is database

    qDebug() <<  QString::number(Patterns.at((i1*6*90) + (j1*90) + (k1))->get_L_debug()) << endl;

    if((i1+1) > 88 && (k1 + 1) > 88  )
    {
    qCritical() << "(i1+1) * (j1+1) * (k1+1) = "+ QString::number((i1+1) * (j1+1) * (k1+1)) << endl;
    }


    double rand_action_boarder = 0.0;
    double rand_value = 0.0;
    int index_to_offer = 0;
    int k_finded=0;

    double grade_debug = 0.0;

    for(int k = 0; k < 90; k++) //k
    {
        for(int j=0; j < 6; j++)
        {
            index_to_offer = k1 * 6 * 90 + j * 90 + k;

            Patterns.at(index_to_offer)->run(pattern_input_to_knowladgebase_offer);
            Patterns.at(index_to_offer)->sync();

            if(index_to_offer!= 0) {
                out_temp = Patterns.at(index_to_offer)->getOUT();
            }


            if(out_temp == 1)
            {

                count++;
                //qCritical() << " out_pattern " + QString::number(out_temp) + " i = " + QString::number(i) << endl;

                //find answer

                   action_answer = j;
                   k_finded = k;
                   goto A;

            }

            Patterns.at(index_to_offer)->reset(); //reset because it is database
        }
    }

    A:


    QVector<int> test;
    test = sortend_input_pattern.at(k1).first;
    grade_debug = evaluationSystem->gradePattern( test );





     qDebug() << "count = " + QString::number(count) << endl;

    QDateTime finish = QDateTime::currentDateTime();

    int msecs = finish.time().msecsTo(start.time());



    //rand on not rand



        debug_text = QString::number(filled_layer, 'f', 15) + "grade = "+ QString::number(grade_debug,'f', 15) + " time in ms : " + QString::number(msecs);

        save_grade_pattern("../grade.txt",grade_debug);

    if(action_answer == -1)
    {
        //rand
        rand_value = static_cast <double> (rand()) / static_cast <float> (RAND_MAX);
        rand_value = rand_value * 100.0;

        rand_action_boarder = probability_of_choosing_a_random_action(90, k_finded);

        if(rand_value >= rand_action_boarder)
        {
            action_answer  = qrand() % ((5 + 1) - 0) + 0;
        }



    }


    return action_answer;


}

//-------------------------------------------------------------------------------------------------
double NeuronKnowledgeBase::probability_of_choosing_a_random_action(int max_length, int current_index)
{
    double test_1, test_2, test_3;
    test_1 = atan(((double)current_index - (double)(max_length)/2) / M_PI);
    test_2 = test_1 * 25;
    test_3 = test_2 + 50.0;

    return  test_3;
    //return  100.0 - test_3;
}

//-------------------------------------------------------------------------------------------------

void NeuronKnowledgeBase::save_KnowledgeBase_to_file(QString path)
{

    QString Hfilename = path;

    QFile fileH( Hfilename );

    if(fileH.open(QIODevice::WriteOnly ))
    {
        QTextStream stream( &fileH );
        if(!Patterns.empty())
        {
            for(int i = 0; i < Patterns.size(); i++)
            {
                stream << QString::number(Patterns.at(i)->get_L_debug()) + "\n";

            }
        }
        fileH.close();

    }


}

void NeuronKnowledgeBase::save_grade_pattern(QString path, double grade_debug)
{
    QString Hfilename = path;

    QFile fileH( Hfilename );

    if(fileH.open(QIODevice::Append ))
    {
        QTextStream stream( &fileH );

        stream << QString::number(grade_debug) + "\n";

        fileH.close();

    }
}


void NeuronKnowledgeBase::load_knowladgebase_from_file(QString path)
{
    QString Hfilename = path;

    QFile fileH( Hfilename );

    if(fileH.open(QIODevice::ReadOnly ))
    {
        QTextStream stream( &fileH );
        if(!Patterns.empty())
        {
            for(int i = 0; i < Patterns.size(); i++)
            {
                  Patterns.at(i)->set_L_debug(stream.readLine().toInt());
            }
        }
        fileH.close();

    }

}
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------


QString NeuronKnowledgeBase::get_debug_text()
{
    return debug_text;
}


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

