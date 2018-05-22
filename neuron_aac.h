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
    neuron_AAC_type_1(QString id, int porog_g, int porog_l, float p_min, float p_max);
    void append_out_neurons(const std::shared_ptr<neuron_AAC_type_1>& neuron);
    void append_in_neurons(const std::shared_ptr<neuron_AAC_type_1>& neuron);
    std::vector<std::shared_ptr<neuron_AAC_type_1>> get_in_neurons();
    std::vector<std::shared_ptr<neuron_AAC_type_1>> get_out_neurons();

    void set_in_neurons(std::shared_ptr<neuron_AAC_type_1> neuron, int index);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    void setLevel(QString level_str);
    QString getLevel();
    QString getId();

    double porog();
    int block_R();
    int block_L();
    void block_T();
    void block_G();
    void run(std::vector<int> FIRST_IMAGE);
    void run();
    void sync();
    void reset();
    void setDrawX(int drawx);
    void setDrawY(int drawy);
    int getDrawX();
    int getDrawY();

private:
    QString ID;
    std::vector<int> IMAGE;
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
