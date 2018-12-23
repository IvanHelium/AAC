#include "robot_simulator.h"




robot_simulator::robot_simulator(QObject *parent) : QObject(parent)
{
    for(int i = 0; i < 90; i++)
    {
        for(int j = 0; j < 6; j++)
        {
            for(int k = 0; k < 90; k++)
            {
                mass[i][j][k] = -1;
            }
        }
    }

    //load here

}



void robot_simulator::save_combination(QVector<int> pattern_current, int action_number, QVector<int> pattern_result)
{



}
