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
    void set_data(int i,int j, int value);


private:
    int size;
    QVector<QVector<int>> data;
};


class Fro
{
public:
    Fro();
    Fro(int size, int tr);
    void init_fro();
    int getFroSize();
    void setFroSize(int size);
    void appendReceptor(std::shared_ptr<neuron_AAC_type_1> candidate);
    std::shared_ptr<neuron_AAC_type_1> getNeuron(int index);
    void defineMAP(std::shared_ptr<neuron_AAC_type_1> neuron);
    bool isPair(std::shared_ptr<neuron_AAC_type_1> neuron1, std::shared_ptr<neuron_AAC_type_1> neuron2); //проверить

    bool check_same(std::vector<std::shared_ptr<neuron_AAC_type_1>> arr1, std::vector<std::shared_ptr<neuron_AAC_type_1>> arr2); // проверить

    QVector<int> create_map(std::shared_ptr<neuron_AAC_type_1> neuron1, std::shared_ptr<neuron_AAC_type_1> neuron2);

    std::shared_ptr<neuron_AAC_type_1> create_new_neuron(QString ID, QString LEVEL);

    void generator(void);


    bool vectors_equal(QVector<int> vector1, QVector<int> vector2);

    void link(QList<std::shared_ptr<neuron_AAC_type_1>> net, int form, int to);


    int getTr();

    void update_matrix_event();

    int run(QVector <int> input_vector);

    QList<std::shared_ptr<neuron_AAC_type_1>> getNeurons();

private:
    int froSize;
    int receptorSize;
    int neuronsCount;
    matrixEvent *ME;
    QList<std::shared_ptr<neuron_AAC_type_1>> receptors;
    QList<std::shared_ptr<neuron_AAC_type_1>> neurons;
    int self_Tr;

    QVector<int> activatedObrazs;
};







#endif // FRO_H
