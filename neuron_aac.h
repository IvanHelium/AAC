#ifndef NEURON_AAC_H
#define NEURON_AAC_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <memory>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>


class neuron_AAC_type_1
{
public:
    neuron_AAC_type_1();
    neuron_AAC_type_1(QString type, QString id,QString level, int porog_g, int porog_l, float p_min, float p_max, int out_porog);
    void append_out_neurons(const std::shared_ptr<neuron_AAC_type_1>& neuron);
    void append_in_neurons(const std::shared_ptr<neuron_AAC_type_1>& neuron);
    std::vector<std::shared_ptr<neuron_AAC_type_1>> get_in_neurons();
    std::vector<std::shared_ptr<neuron_AAC_type_1>> get_out_neurons();

    void set_in_neurons(std::shared_ptr<neuron_AAC_type_1> neuron, int index);
    void setMAP(QVector<int> Map);
    QVector<int> getMAP();

    void setImage(QVector<int> image);


    void setOBRAZ(QVector<int> Obraz);

    QString getTYPE();

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    void setLevel(QString level_str);
    QString getLevel();
    QString getId();

    int getOUT();

    double porog();
    int block_R();
    int block_L();
    void block_T();
    void block_G();
    void run(QVector<int> FIRST_IMAGE);
    void run();
    void sync();
    void reset();
    void setDrawX(int drawx);
    void setDrawY(int drawy);
    int getDrawX();
    int getDrawY();

    int get_L_debug();
    void set_L_debug(int value);

private:
    QString ID;
    QVector<int> IMAGE;
    int STEP;
    int G;
    int L;
    int ADAPTED;
    int B;
    int C;
    int OUT;
    int NEXT_OUT;
    int POROG_G;
    int POROG_L;
    int S; // obratnii signal ot drugogo neirona
    int NEXT_S;
    int Z; // obratnii signal ot sebya
    int NEXT_Z;
    float P_MAX;
    float P_MIN;
    std::vector<std::shared_ptr<neuron_AAC_type_1>> in_neurons;
    std::vector<std::shared_ptr<neuron_AAC_type_1>> out_neurons;
    QString TYPE;

    QString LEVEL;

    int drawX;
    int drawY;

    QVector<int> MAP;
    QVector<int> OBRAZ;

    int OUT_POROG;
    int OUT_TAKT;


};



class receptor_AAC
{
public:
    receptor_AAC(std::string id,int image, int step, int out);
    void setImage(int image);
    void sync();
    void reset();
    void run();

private:
    std::string ID;
    int IMAGE;
    int STEP;
    int OUT;
    int NEXT_OUT;
    std::string TYPE;

};

#endif // NEURON_AAC_H
