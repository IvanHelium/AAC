#include "robot_simulator.h"




Robot_simulator::Robot_simulator(QObject *parent) : QObject(parent)
{
    for(int i = 0; i < 90; i++) //filled by zeros
    {
        for(int j = 0; j < 6; j++)
        {
            for(int k = 0; k < 90; k++)
            {
                mass[i][j][k] = 0;
            }
        }
    }

    input_patterns_list = generate_test_data();

    //load here

}

//--------------------------------------------------------------------------------

void Robot_simulator::save_combination(QVector<int> pattern_current, int action_number, QVector<int> pattern_result)
{
    int m_i = -1;
    int m_j = -1;
    int m_k = -1;

    for(int i = 0; i < 90; i++)
    {
        if(pattern_current == input_patterns_list.at(i))
        {
            m_i = i; // pattern
        }
    }

    m_j = action_number;


    for(int i = 0; i < 90; i++)
    {
        if(pattern_result == input_patterns_list.at(i))
        {
            m_k = i; // pattern
        }
    }


    if((m_i > -1) && (m_j > -1) && (m_k > -1))
    {
        mass[m_i][m_j][m_k]++;
    }

}




//--------------------------------------------------------------------------------

QVector<QVector<int>> Robot_simulator::generate_test_data()
{
    QVector<QVector<int>> testrun;// =  {1,0,1,0,0,0,0,1,0,1};

    QVector<int> vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};



        for(int j = 0; j < 5; j++)
        {



            for(int k = 5; k < 11; k++)
            {


                for(int t = 11; t < 14; t++)
                {
                    vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                    vector[j] = 1;
                    vector[k] = 1;
                    vector[t] = 1;

                    testrun.append(vector);
                }
            }
        }



return testrun;

}

//--------------------------------------------------------------------------------

QVector<QVector<int>> Robot_simulator::get_generated_test_data()
{
    return input_patterns_list;
}



//--------------------------------------------------------------------------------
void Robot_simulator::save_to_file(QString path)
{
    QString Hfilename = path;

    QFile fileH( Hfilename );

    if(fileH.open(QIODevice::WriteOnly ))
    {
        QTextStream stream( &fileH );

        for(int i =0; i < 90; i++)
        {
            for(int j =0; j< 6; j++)
            {
               for(int k =0; k < 90; k++)
               {
                stream << QString::number(mass[i][j][k]) + "\n";
               }
            }
        }


        fileH.close();

    }
}
//--------------------------------------------------------------------------------

void Robot_simulator::load_to_file(QString path)
{
    QString Hfilename = path;

    QFile fileH( Hfilename );

    if(fileH.open(QIODevice::ReadOnly ))
    {
        QTextStream stream( &fileH );

        for(int i =0; i < 90; i++)
        {
            for(int j =0; j< 6; j++)
            {
               for(int k =0; k < 90; k++)
               {
                mass[i][j][k] =  stream.readLine().toInt();
               }
            }
        }
        fileH.close();

    }
}
