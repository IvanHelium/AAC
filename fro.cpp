#include "fro.h"



Fro::Fro()
{

}

//----------------------------------------------------------------------
Fro::Fro(int size, int tr)
{
    froSize = size;
    receptorSize = size;
    ME = new matrixEvent(size);
    neuronsCount = 0;
    self_Tr = tr;

    for(int i = 0; i< size; i++)
    {
    activatedObrazs.append(0);
    }

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
//----------------------------------------------------------------------
bool Fro::isPair(std::shared_ptr<neuron_AAC_type_1> neuron1, std::shared_ptr<neuron_AAC_type_1> neuron2)
{
    bool result;
    result = check_same(neuron1->get_in_neurons(), neuron2->get_in_neurons());
    return result;
}

bool Fro::intersectingMap(QVector<int> neuron_map1, QVector<int> neuron_map2)
{
    int mapSize = receptorSize;

    for(int i = 0; i < receptorSize; i++)
    {
         if((neuron_map1.at(i) == 1) && (neuron_map2.at(i) == 1))
         {
            return false;
         }
    }
    return true;
}
//----------------------------------------------------------------------
bool Fro::check_same(std::vector<std::shared_ptr<neuron_AAC_type_1>> arr1, std::vector<std::shared_ptr<neuron_AAC_type_1>> arr2)
{
    for(int i = 0; i < arr1.size(); i++)
    {
        for(int j = 0; j < arr2.size(); j++)
        {
            if(arr1[i]->getId() == arr2[j]->getId())
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
  std::shared_ptr<neuron_AAC_type_1> neuron = std::shared_ptr<neuron_AAC_type_1> (new neuron_AAC_type_1("neuron_type_1", ID, LEVEL, 3, 3, 0.8, 0.9, self_Tr));
  neurons.append(neuron);
  froSize++;

  activatedObrazs.append(0);

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

void Fro::link(QList<std::shared_ptr<neuron_AAC_type_1>> net, int form, int to)
{
    net.at(form)->append_out_neurons(net.at(to));
    net.at(to)->append_in_neurons(net.at(form));
}
//----------------------------------------------------------------------
int Fro::getTr()
{
    return self_Tr;
}

//----------------------------------------------------------------------
void Fro::generator(void)
{
   bool b = true;
   QVector<QPair<int,int>> P;
   QVector<int> neuron_map;
   QString neuron_ID;
   int LEVEL;
   std::shared_ptr<neuron_AAC_type_1> neuron;

   P =  ME->get_candidate(20);

   for(int i = 0; i < P.size(); i++)
   {
        neuron_map = create_map(neurons.at(P.at(i).first), neurons.at(P.at(i).second));
        b = true;

        b = isPair(neurons.at(P.at(i).first), neurons.at(P.at(i).second));
        b = intersectingMap(neurons.at(P.at(i).first)->getMAP(), neurons.at(P.at(i).second)->getMAP());

        for(int j = receptorSize; j < neurons.size();j++)
        {

            if(vectors_equal(neuron_map, neurons.at(j)->getMAP()))
            {
                b = false;
                break;
            }
            if(intersectingMap(neuron_map, neurons.at(j)->getMAP()))
            {
                b = false;
                break;
            }




        }

        if(b)
        {
            neuron_ID = QString::number(froSize);
            LEVEL =  neurons.at(P.at(i).first)->getLevel().toInt() + 1;

            if(  neurons.at(P.at(i).second)->getLevel().toInt() >  neurons.at(P.at(i).first)->getLevel().toInt())
            {
                LEVEL  = neurons.at(P.at(i).second)->getLevel().toInt() + 1;
            }

            //create neuron
            neuron = create_new_neuron(neuron_ID,QString::number(LEVEL));
            neuron->setMAP(neuron_map);
            neuron->setOBRAZ(neuron_map);

            //???
            neuron->append_in_neurons(neurons.at(P.at(i).first));
            link(neurons, P.at(i).first , neuron_ID.toInt());
            link(neurons, P.at(i).second , neuron_ID.toInt());

            ME->pad();
            ME->set_data(P.at(i).first, P.at(i).second, 0);
            ME->set_data(P.at(i).second, P.at(i).first, 0);

        }



   }
}



//----------------------------------------------------------------------


void Fro::update_matrix_event()
{
    QVector<int> vector;
    for(int i = 0; i < activatedObrazs.size(); i++)
    {
        vector.append(0);
    }

    for(int j = 0; j < activatedObrazs.size(); j++)
    {
        vector[j] = activatedObrazs[j];
    }

    ME->update(vector);

}

//----------------------------------------------------------------------


int Fro::run(QVector <int> input_vector)
{
    QVector<int> temp_image;
    temp_image.clear();
    if(input_vector.size() != receptorSize)
    {
        return -1;
    }

    //refresh receptors
    for(int i = 0; i < receptorSize; i++)
    {
        temp_image.append(input_vector[i]);
        receptors.at(i)->setImage(temp_image);
        receptors.at(i)->run(temp_image);
        receptors.at(i)->sync();
        temp_image.clear();
    }


    for(int i = receptorSize; i < froSize; i++)
    {
        neurons.at(i)->run();
    }

    for(int i = receptorSize; i < froSize; i++)
    {
        neurons.at(i)->sync();
    }

    for(int i = 0; i < froSize; i++)
    {
        activatedObrazs[i] += neurons.at(i)->getOUT();
    }

    update_matrix_event();
    generator();

    return 0;
}


 QList<std::shared_ptr<neuron_AAC_type_1>> Fro::getNeurons()
 {
     return neurons;
 }

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
void matrixEvent::set_data(int i,int j, int value)
{
    data[i][j] = value;
}

//------------------------------------------------------------------
