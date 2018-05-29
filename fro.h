#ifndef FRO_H
#define FRO_H

#include <QObject>
#include <QVector>
#include "neuron_aac.h"


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


class Fro
{
public:
    Fro();
    Fro(int size);
    void init_fro();
    int getFroSize();
    void setFroSize(int size);
    void appendReceptor(std::shared_ptr<neuron_AAC_type_1> candidate);
    std::shared_ptr<neuron_AAC_type_1> getNeuron(int index);
    void defineMAP(std::shared_ptr<neuron_AAC_type_1> neuron);

private:
    int froSize;
    int receptorSize;
    int neuronsCount;
    matrixEvent *ME;
    QList<std::shared_ptr<neuron_AAC_type_1>> receptors;
    QList<std::shared_ptr<neuron_AAC_type_1>> neurons;
};







#endif // FRO_H
