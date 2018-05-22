#ifndef FRO_H
#define FRO_H

#include <QObject>
#include <QVector>
#include "neuron_aac.h"

class fro
{
public:
    fro();
};





class matrixEvent
{
public:
    matrixEvent();
    matrixEvent(int sizeI);
    void pad();
    void reset();
    void update(QVector<int> vec);
    QVector<QPair<int, int> > get_candidate(int L);

private:
    int size;
    QVector<QVector<int>> data;
};

#endif // FRO_H
