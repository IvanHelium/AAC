#include "neuron_aac.h"
#include "math.h"
#include <iostream>


QJsonObject ObjectFromString(const QString& in)
{
    QJsonValue val(in);
    return val.toObject();
}

neuron_AAC_type_1::neuron_AAC_type_1()
{
    ID = "";
    IMAGE.clear();
    STEP = 0;
    G = 0;
    L = 0;
    ADAPTED=0;
    B=0;
    C=0;
    OUT=0;
    NEXT_OUT = 0;
    POROG_G = 0;
    POROG_L = 0;
    S = 0; // obratnii signal ot drugogo neirona
    NEXT_S = 0;
    Z = 0; // obratnii signal ot sebya
    NEXT_Z = 0;
    P_MAX = 0.0f;
    P_MIN = 0.0f;
    in_neurons.clear();
    out_neurons.clear();
    TYPE = "neuron_type_1";
    LEVEL = "";

    drawX = 0;
    drawY = 0;

    MAP.append(0);
}
neuron_AAC_type_1::neuron_AAC_type_1(QString id, int porog_g, int porog_l, float p_min, float p_max)
{
    ID = id;
    IMAGE.clear();
    STEP = 0;
    G = 0;
    L = 0;
    ADAPTED=0;
    B=0;
    C=0;
    OUT=0;
    NEXT_OUT = 0;
    POROG_G = porog_g;
    POROG_L = porog_l;
    S = 0; // obratnii signal ot drugogo neirona
    NEXT_S = 0;
    Z = 0; // obratnii signal ot sebya
    NEXT_Z = 0;
    P_MAX = p_max;
    P_MIN = p_min;
    in_neurons.clear();
    out_neurons.clear();
    TYPE = "neuron_type_1";

    LEVEL = "";

    drawX = 0;
    drawY = 0;

    MAP.append(0);


}


void neuron_AAC_type_1::append_out_neurons(const std::shared_ptr<neuron_AAC_type_1>& neuron)
{
    out_neurons.push_back(neuron);
}

void neuron_AAC_type_1::append_in_neurons(const std::shared_ptr<neuron_AAC_type_1>& neuron)
{
    in_neurons.push_back(neuron);
}


double neuron_AAC_type_1::porog()
{
 double p_N = (1/(1+exp(L-2*POROG_L)))*(P_MAX-P_MIN)+P_MIN;

 return p_N;

}


int neuron_AAC_type_1::block_R()
{
    B=0;

    double p_N_1 = porog();
    std::vector<int> sign = IMAGE;
    double p_sign = 0;

    for(unsigned int i = 0; i < sign.size(); i++)
    {
        p_sign += sign[i];
    }
    p_sign = p_sign / sign.size();

    if(p_sign >= p_N_1)
    {
        B = 1;
    }
    return B;
}

int neuron_AAC_type_1::block_L()
{
 C = 0;
     if(B)
     {
         L++;
         if(L>=POROG_L)
         {
            C = 1;
            ADAPTED = 1;
         }
     }
     return C;
}

void neuron_AAC_type_1::block_G()
{
    Z = 0;
    if(ADAPTED && C)
    {
       G++;
    }
}

void neuron_AAC_type_1::block_T()
{
    if(!S)
    {
        if(C)
        {
            NEXT_OUT = 1;
        }
    }
    else
    {
        NEXT_OUT = 0;
    }
}

void neuron_AAC_type_1::run(std::vector<int> FIRST_IMAGE)
{
    IMAGE.clear();


    IMAGE = FIRST_IMAGE;

    block_R();
    block_L();
    block_T();
    block_G();
}

void neuron_AAC_type_1::run()
{
        IMAGE.clear();


    for(auto x: in_neurons)
    {
        IMAGE.push_back(x->OUT);
    }

    block_R();
    block_L();
    block_T();
    block_G();
}

void neuron_AAC_type_1::sync()
{
 STEP++;
 S = NEXT_S;
 NEXT_S = 0;
 OUT = NEXT_OUT;

}

void neuron_AAC_type_1::reset()
{

    OUT = 0;
    NEXT_OUT = 0;

}

void neuron_AAC_type_1::setDrawX(int drawx)
{
    drawX = drawx;
}

void neuron_AAC_type_1::setDrawY(int drawy)
{
    drawY = drawy;
}

int neuron_AAC_type_1::getDrawX()
{
    return drawX;
}
int neuron_AAC_type_1::getDrawY()
{
    return drawY;
}

std::vector<std::shared_ptr<neuron_AAC_type_1>> neuron_AAC_type_1::get_in_neurons()
{
    return in_neurons;
}
void neuron_AAC_type_1::set_in_neurons(std::shared_ptr<neuron_AAC_type_1> neuron, int index)
{
    in_neurons[index] = neuron;

}

std::vector<std::shared_ptr<neuron_AAC_type_1>> neuron_AAC_type_1::get_out_neurons()
{
    return out_neurons;
}

void neuron_AAC_type_1::setLevel(QString level_str)
{
    LEVEL = level_str;
}
QString neuron_AAC_type_1::getLevel()
{
    return LEVEL;
}

QString neuron_AAC_type_1::getId()
{
    return ID;
}


void neuron_AAC_type_1::read(const QJsonObject &json)
{

}
void neuron_AAC_type_1::write(QJsonObject &json) const
{
        json["ID"] = ObjectFromString(ID);
        //json["IMAGE"] = IMAGE;
        json["G"] = G;
        json["L"] = L;
        json["ADAPTED"] = ADAPTED;
        json["B"] = B;
        json["C"] = C;
        json["OUT"] = OUT;
        json["POROG_G"] = POROG_G;
        json["POROG_L"] = POROG_L;
        json["S"] = S;
        json["Z"] = Z;
        json["P_MAX"] = P_MAX;
        json["P_MIN"] = P_MIN;


}

//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------


receptor_AAC::receptor_AAC(std::string id,int image, int step, int out)
{
    ID = id;
    IMAGE = image;
    STEP = step;
    OUT = out;
    NEXT_OUT = out;
    TYPE = "receptor";
}

void receptor_AAC::setImage(int _image)
{
    IMAGE = _image;
    NEXT_OUT = IMAGE;
}

void receptor_AAC::sync()
{
    STEP++;
    OUT = NEXT_OUT;
}

void receptor_AAC::reset()
{
    OUT = 0;
    NEXT_OUT = 0;
    IMAGE = 0;
}

void receptor_AAC::run()
{
    //return;
}

