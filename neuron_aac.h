#ifndef NEURON_AAC_H
#define NEURON_AAC_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <memory>

class neuron_AAC_type_1
{
public:
    neuron_AAC_type_1();
    neuron_AAC_type_1(std::string id, int porog_g, int porog_l, float p_min, float p_max);
    void append_out_neurons(std::shared_ptr<neuron_AAC_type_1>& neuron);
    void append_in_neurons(std::shared_ptr<neuron_AAC_type_1>& neuron);
    double porog();
    int block_R();
    int block_L();
    void block_T();
    void block_G();
    void run(std::vector<int> FIRST_IMAGE);
    void run();
    void sync();
    void reset();

private:
    std::string ID;
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
    int size_in;
    std::vector<std::shared_ptr<neuron_AAC_type_1>> out_neurons;
    int size_out;
    std::string TYPE;


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