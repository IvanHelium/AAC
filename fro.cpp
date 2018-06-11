#include "fro.h"



Fro::Fro()
{

}

//----------------------------------------------------------------------
Fro::Fro(int size)
{
    froSize = size;
    receptorSize = size;
    ME = new matrixEvent(size);
    neuronsCount = 0;

}

//----------------------------------------------------------------------
void Fro::appendReceptor(std::shared_ptr<neuron_AAC_type_1> candidate)
{
 neuronsCount++;
 receptors.append(candidate);
 neurons.append(candidate);
}

//----------------------------------------------------------------------
int Fro::getFroSize()
{
    return froSize;
}
//----------------------------------------------------------------------
std::shared_ptr<neuron_AAC_type_1> Fro::getNeuron(int index)
{
    return neurons[index];
}
//----------------------------------------------------------------------
void Fro::defineMAP(std::shared_ptr<neuron_AAC_type_1> neuron)
{
    QVector<int> tempMAP;
    tempMAP.clear();

    if(neuron->getTYPE() == "receptor")
    {
        for(int i = 0; i < this->receptorSize; i++)
        {

            if(neuron->getId().toInt() == i)
            {
                tempMAP.append(1);
            }
            else
            {
                tempMAP.append(0);
            }
        }
    }

    neuron->setMAP(tempMAP);

}
//----------------------------------------------------------------------
bool Fro::isPair(std::shared_ptr<neuron_AAC_type_1> neuron1, std::shared_ptr<neuron_AAC_type_1> neuron2)
{
    bool result;
    result = check_same(neuron1->get_in_neurons(), neuron2->get_in_neurons());
    return result;
}
//----------------------------------------------------------------------
bool Fro::check_same(std::vector<std::shared_ptr<neuron_AAC_type_1>> arr1, std::vector<std::shared_ptr<neuron_AAC_type_1>> arr2)
{
    for(int i = 0; i < arr1.size(); i++)
    {
        for(int j = 0; j < arr2.size(); j++)
        {
            if(arr1[i] == arr2[j])
            {
                return false;
            }
        }
    }
    return true;
}
//----------------------------------------------------------------------
QVector<int>  Fro::create_map(std::shared_ptr<neuron_AAC_type_1> neuron1, std::shared_ptr<neuron_AAC_type_1> neuron2)
{
  QVector<int> temp_MAP;
  temp_MAP.resize(receptorSize);
  QVector<int> newVec1 = neuron1->getMAP();
  QVector<int> newVec2 = neuron2->getMAP();

  for(int i = 0; i < receptorSize; i++)
  {
      if(newVec1[i] >= 1 || newVec2[i] >= 1)
      {
        temp_MAP[i] = 1;
      }
  }
  return temp_MAP;

}
//----------------------------------------------------------------------


std::shared_ptr<neuron_AAC_type_1> Fro::create_new_neuron(QString ID, QString LEVEL)
{
  std::shared_ptr<neuron_AAC_type_1> neuron = std::shared_ptr<neuron_AAC_type_1> (new neuron_AAC_type_1("neuron_type_1", ID, LEVEL, 3, 3, 0.8, 0.9));
  neurons.append(neuron);
  froSize++;

return neuron;
}
//----------------------------------------------------------------------
bool Fro::vectors_equal(QVector<int> vector1, QVector<int> vector2)
{
    bool result = false;

    if(vector1.size() != vector2.size())
        return result;

    for(int i = 0; i < vector1.size(); i++)
    {
        if(vector1.at(i) != vector2.at(i))
        {
            return result;
        }
    }
    result = true;
    return result;
}

//----------------------------------------------------------------------
void Fro::generator(void)
{
   bool b = true;
   QVector<QPair<int,int>> P;
   QVector<int> neuron_map;

   P =  ME->get_candidate(20);

   for(int i = 0; i < P.size(); i++)
   {
        neuron_map = create_map(neurons.at(P.at(i).first), neurons.at(P.at(i).second));
        b = true;

        b = isPair(neurons.at(P.at(i).first), neurons.at(P.at(i).second));

        for(int j = 0; j < neurons.size();j++)
        {

            if(vectors_equal(neuron_map, neurons.at(j)->getMAP()))
            {
                b = false;
            }
        }



   }
}



//----------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
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
