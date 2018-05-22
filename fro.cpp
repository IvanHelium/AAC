#include "fro.h"





fro::fro()
{

}








//--------------------------------------------------------------


matrixEvent::matrixEvent()
{
    QVector<int> vec2;
    size = 3;

    for(int j = 0; j < 3; j++)
    {
        vec2.append(0);
    }
    for(int i = 0; i < 3; i++)
    {
        data.append(vec2);
    }
}

//------------------------------------------------------------------
matrixEvent::matrixEvent(int sizeI)
{
    QVector<int> vec2;

    size = sizeI;
    for(int j = 0; j < size; j++)
    {
        vec2.append(0);
    }

    for(int i = 0; i < size; i++)
    {
        data.append(vec2);
    }
}

//------------------------------------------------------------------
void matrixEvent::pad()
{

    QVector<int> vec2;

    for(int i = 0; i < size; i++)
    {
        vec2 = data.at(i);
        vec2.append(0);
        data.replace(i,vec2);
    }

    data.append(vec2);
    size++;

}
//------------------------------------------------------------------
void matrixEvent::reset()
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            data[i][j] = 0;
        }
    }
}
//------------------------------------------------------------------

void matrixEvent::update(QVector<int> vec)
{
    int n = vec.size();
    if(n > size)
    {
        n = size;
    }
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(vec[i] == vec[j])
            {
                data[i][j] += vec[i];
            }
        }
    }
}


//------------------------------------------------------------------
QVector<QPair<int,int>> matrixEvent::get_candidate(int L)
{
    QVector<QPair<int,int>> P;
    P.clear();

    for(int i = 0; i< data[0].size(); i++)
    {
        for(int j =0; j < i; j++)
        {
            if((data[i][j] > L) && (i != j))
            {
                P.push_back(QPair<int,int>(i,j));
            }
        }
    }

    return P;


}

//------------------------------------------------------------------
