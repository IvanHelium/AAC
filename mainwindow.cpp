#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "neuron_aac.h"
#include <QDebug>
#include <QThread>
#include <QSerialPortInfo>
#include <QTimer>
#include <QComboBox>
#include <QScrollBar>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QtGui>
#include <QPoint>







bool connect_check=false;
QString netFileName = "netfile.txt";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    serial(new SerialPort(this))

{
    ui->setupUi(this);

    ui->comboBox->setModel(&portNamesModel);
    QStringList portNames = serial->getPortNames();
    portNamesModel.setStringList(portNames);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_update()));
    timer->start(5000);

    QTimer *timer_action = new QTimer(this);
    connect(timer_action, SIGNAL(timeout()), this, SLOT(timer_action_update()));
    timer_action->start(2000);



    connect(serial, &SerialPort::sensorDataSignal, this, &MainWindow::debugSensorView);

    connect(serial, &SerialPort::sensorDataSignal, this, &MainWindow::prase_sensor_data);



    /*n01 = neuron_AAC_type_1("n01", 3, 3, 0.9, 0.9);

    n02 = neuron_AAC_type_1("n02", 3, 3, 0.9, 0.9);

    n03 = neuron_AAC_type_1("n03", 3, 3, 0.9, 0.9);
    std::shared_ptr<neuron_AAC_type_1> n04(new neuron_AAC_type_1("n04", 3,3,0.9, 0.9));

    std::shared_ptr<neuron_AAC_type_1> ptr_n01(&n02);
    n01.append_out_neurons(ptr_n01);
    n01.append_out_neurons(n04);*/

    /*std::shared_ptr<neuron_AAC_type_1> n01(new neuron_AAC_type_1("n01", 3, 3, 0.9, 0.9));

    std::shared_ptr<neuron_AAC_type_1> n02(new neuron_AAC_type_1("n02", 3, 3, 0.9, 0.9));

    std::shared_ptr<neuron_AAC_type_1> n03(new neuron_AAC_type_1("n03", 3, 3, 0.9, 0.9));*/

    int firstFroSize = 14;
    fro_test = new Fro(firstFroSize, 5);

    for(int i = 0; i < firstFroSize; i++)
    {
        fro_test->appendReceptor(std::shared_ptr<neuron_AAC_type_1> (new neuron_AAC_type_1("receptor", QString::number(i),"0", 3, 3, 0.8, 0.9, fro_test->getTr())));

    }


    for(int i = 0; i < firstFroSize; i++)
    {
    fro_test->defineMAP(fro_test->getNeuron(i));
    }


    int stepX = 40;
    int stepY = 40;

    for(int i = 0; i < 9; i++)
    {
      net2.append(std::shared_ptr<neuron_AAC_type_1> (new neuron_AAC_type_1("neuron_type_1", QString::number(i),"0", 3, 3, 0.8, 0.9, 0)));
    }

    net2.at(0)->setDrawX(stepX);
    net2.at(0)->setDrawY(stepY);
    net2.at(3)->setDrawX(stepX);
    net2.at(3)->setDrawY(2*stepY);
    net2.at(6)->setDrawX(stepX);
    net2.at(6)->setDrawY(3*stepY);
    net2.at(8)->setDrawX(stepX);
    net2.at(8)->setDrawY(4*stepY);

    net2.at(1)->setDrawX(2*stepX);
    net2.at(1)->setDrawY(stepY + stepY/2);
    net2.at(4)->setDrawX(2*stepX);
    net2.at(4)->setDrawY(2*stepY + stepY/2);
    net2.at(7)->setDrawX(2*stepX);
    net2.at(7)->setDrawY(3*stepY + stepY/2);

    net2.at(2)->setDrawX(3*stepX);
    net2.at(2)->setDrawY(stepY);
    net2.at(5)->setDrawX(3*stepX);
    net2.at(5)->setDrawY(2*stepY);


    //link(net2, 0, 1);
    //link(net2, 1, 2);
    //link(net2, 3, 1);
    //link(net2, 4, 2);
    //link(net2, 3, 4);
    //link(net2, 4, 5);
    //link(net2, 6, 4);
    //link(net2, 7, 5);
    //link(net2, 6, 7);
    //link(net2, 8, 7);

    defineStructure(net2);

    //bool test = fro_test->check_same(net2.at(2)->get_in_neurons(), net2.at(5)->get_in_neurons());

   /* fro_test->create_new_neuron( QString::number(fro_test->getFroSize()), "2");
    drawNetOnLabel(net2);


    QVector<int> test1 = {1,1,0,0,1};
    QVector<int> test2 = {1,1,0,0};

     bool test = fro_test->vectors_equal(test1,test2);*/

    //std::vector <int> FIRST_IMAGE;

    //FIRST_IMAGE.push_back(1);
    //FIRST_IMAGE.push_back(1);
    //FIRST_IMAGE.push_back(1);
    //FIRST_IMAGE.push_back(1);
    //FIRST_IMAGE.push_back(1);


/*for(int i = 0; i < 11; i++)
{

    n01->run(FIRST_IMAGE);
    n01->sync();
    n02->run();
    n02->sync();
    n03->run();
    n03->sync();
}*/
QVector<QVector<int>> testpattern;

testpattern = generate_test_data();


QVector<int> testgrade = { 2000, 2, 10, 4, 10, 1000, 1000, 600, 600, 300, 300, 160, 120, 80 };

evaluationSystem = new EvaluationSystem(true,testgrade);

//evaluationSystem.setInputGrade(testgrade);



QVector<int> testrun =  {1,0,0,0,1,0,0,0,0,0,0,0,0,1};
/*
for(int i = 0; i < firstFroSize; i++)
{
testrun.append(1);
}*/

for(int i = 0; i < 120 * 90; i++)
{
   testrun = testpattern[randInt(0,89)];
  fro_test->run(testrun);
  if(i % 500 == 0)
  {
    formGraphVizText(fro_test->getNeurons());
  }
  qDebug() << "debug still running at i =" << i << endl;
}


formGraphVizText(fro_test->getNeurons());


neuronKnowledgeBase = new NeuronKnowledgeBase(true, testgrade);

}




MainWindow::~MainWindow()
{
    delete ui;
}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void MainWindow::on_actionConnect_triggered(bool checked)
{
    if (checked) {
        connect_check = true;
        if (serial->openPort(ui->comboBox->currentText())) {
            ui->statusBar->showMessage("Serial port opened");
        } else {
            ui->statusBar->showMessage("Could not open serial port");
            ui->actionConnect->setChecked(false);
        }
    } else {
        connect_check = false;
        serial->closePort();
    }
}

//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void MainWindow::timer_update()
{
    QStringList portNames = serial->getPortNames();
    QString currentPort = ui->comboBox->currentText();
    portNamesModel.setStringList(portNames);
    ui->comboBox->setCurrentText(currentPort);

    //serial->sendDriveForward();
    serial->sendDriveBackRight();
}


//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------

void MainWindow::timer_action_update()
{
    QVector<int> FRO_vector_run_current;
    QVector<int> FRO_vector_run_previous;
    int actionIndexPrevious;
    qDebug() << "timer action event " << endl;
    int rand = randInt(0,5);




    if(abs(sensordata.previous_distance - sensordata.current_distance) <=2)
    {
        sensordata.same_distance = 1;
        sensordata.closer_distance = 0;
        sensordata.longer_distance = 0;
    }
    else
    {
        if((sensordata.previous_distance - sensordata.current_distance) > 0)
        {
            sensordata.same_distance = 0;
            sensordata.closer_distance = 1;
            sensordata.longer_distance = 0;

        }
        else
        {
            sensordata.same_distance = 0;
            sensordata.closer_distance = 0;
            sensordata.longer_distance = 1;
        }
    }
    FRO_vector_run_current.clear();
    FRO_vector_run_current.append((int)sensordata.no_sensor);
    FRO_vector_run_current.append((int)sensordata.tactile_forward_sensor);
    FRO_vector_run_current.append((int)sensordata.range_left_sensor);
    FRO_vector_run_current.append((int)sensordata.range_center_sensor);
    FRO_vector_run_current.append((int)sensordata.range_right_sensor);

    FRO_vector_run_current.append((int)sensordata.direction_0_30_left_5);
    FRO_vector_run_current.append((int)sensordata.direction_0_30_right_6);
    FRO_vector_run_current.append((int)sensordata.direction_30_180_left_7);
    FRO_vector_run_current.append((int)sensordata.direction_30_180_right_8);
    FRO_vector_run_current.append((int)sensordata.direction_180_360_left_9);
    FRO_vector_run_current.append((int)sensordata.direction_180_360_right_10);

    FRO_vector_run_current.append((int)sensordata.closer_distance);
    FRO_vector_run_current.append((int)sensordata.same_distance);
    FRO_vector_run_current.append((int)sensordata.longer_distance);


    neuronKnowledgeBase->setKnowledgeBasePatternCurrent(FRO_vector_run_current); //result pattern
    FRO_vector_run_previous = neuronKnowledgeBase->getKnowledgeBasePatternPrevious(); //previous pattern
    actionIndexPrevious = neuronKnowledgeBase->getLastActionIndex(); //previous action

    //run knowladgebase and save and manage

    //and choose action




    neuronKnowledgeBase->setLastActionIndex(actionIndexPrevious);
    neuronKnowledgeBase->setKnowledgeBasePatternPrevious(FRO_vector_run_current);

    //fro_test->run(FRO_vector_run);




    ui->label->setText("number of FRO neurons: " + QString::number(fro_test->getNeurons().size()));
    formGraphVizText(fro_test->getNeurons());

    switch(rand)
    {
      case 0:
        serial->sendDriveForward();
        break;
      case 1:
        serial->sendDriveForwardLeft();
        break;
      case 2:
        serial->sendDriveForwardRight();
        break;
      case 3:
        serial->sendDriveBack();
        break;
      case 4:
        serial->sendDriveBackLeft();
        break;
      case 5:
        serial->sendDriveBackRight();
        break;
      default:
        serial->sendDriveForward();
    }
     sensordata.previous_distance = sensordata.current_distance;

}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
int MainWindow::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------

QVector<QVector<int>> MainWindow::generate_test_data()
{
    QVector<QVector<int>> testrun;// =  {1,0,1,0,0,0,0,1,0,1};

    QVector<int> vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};



        for(int j = 0; j < 5; j++)
        {



            for(int k = 5; k < 11; k++)
            {


                for(int t = 11; t < 14; t++)
                {
                    vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                    vector[j] = 1;
                    vector[k] = 1;
                    vector[t] = 1;

                    testrun.append(vector);
                }
            }
        }



return testrun;

}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------

void MainWindow::prase_sensor_data(uint8_t tact_sensor_data,uint8_t direction_to_resource,uint8_t direction_to_resource_sign, uint8_t distance_to_recource)
{


    sensordata.current_distance = distance_to_recource;


    sensordata.tactile_forward_sensor = tact_sensor_data & 0x01 == 0x01;
    sensordata.range_left_sensor = tact_sensor_data & 0x02 == 0x02;
    sensordata.range_center_sensor = tact_sensor_data & 0x04 == 0x04;
    sensordata.range_right_sensor = tact_sensor_data & 0x08 == 0x08;




            if((sensordata.tactile_forward_sensor == 0) && (sensordata.range_left_sensor == 0) && (sensordata.range_center_sensor == 0) && (sensordata.range_right_sensor == 0))
            {
                sensordata.no_sensor = 1;
            }

    sensordata.direction_0_30_left_5 = 0;
    sensordata.direction_0_30_right_6 = 0;
    sensordata.direction_30_180_left_7 = 0;
    sensordata.direction_30_180_right_8 = 0;
    sensordata.direction_180_360_left_9 = 0;
    sensordata.direction_180_360_right_10 = 0;

    sensordata.no_sensor = 0;

    if(!direction_to_resource_sign)
    {
        if(direction_to_resource >= 0 && direction_to_resource < 30)
        {
            sensordata.direction_0_30_left_5 = 1;
        }
        else if(direction_to_resource >= 30 && direction_to_resource < 180)
        {
            sensordata.direction_30_180_left_7 = 1;
        }
        else if(direction_to_resource >= 180 && direction_to_resource < 360)
        {
            sensordata.direction_180_360_left_9 = 1;
        }


    }
    else
    {
        if(direction_to_resource >= 0 && direction_to_resource < 30)
        {
            sensordata.direction_0_30_right_6 = 1;
        }
        else if(direction_to_resource >= 30 && direction_to_resource < 180)
        {
            sensordata.direction_30_180_right_8 = 1;
        }
        else if(direction_to_resource >= 180 && direction_to_resource < 360)
        {
            sensordata.direction_180_360_right_10 = 1;
        }
    }






}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void MainWindow::debugSensorView(uint8_t tact_sensor_data,uint8_t direction_to_resource,uint8_t direction_to_resource_sign, uint8_t distance_to_recource)
{
    qDebug() << "sensor  = " << QString::number(tact_sensor_data) << "direction = " << QString::number(direction_to_resource) << "\n  sign" << QString::number(direction_to_resource_sign)<< "\n  distance" << QString::number(distance_to_recource);
}


void MainWindow::drawNetOnLabel(QList<std::shared_ptr<neuron_AAC_type_1>> net)
{
    int circleRadius = 10;
    QPixmap *pix1 = new QPixmap(500,500);
    //pix1.fill( Qt::red );
    pix1->fill( Qt::white);
    QPainter *paint = new QPainter(pix1);
    paint->setPen(*(new QColor(0,0,0,255)));

    QPoint *point = new QPoint(30, 100);
    QPoint *newPoint = new QPoint(0, 0);
    QPoint *newPoint2 = new QPoint(0, 0);

    for(int i = 0; i < net.size(); i++)
    {
        newPoint->setX(point->x() + net.at(i)->getDrawX());
        newPoint->setY(point->y() + net.at(i)->getDrawY());
         paint->drawEllipse(*newPoint, circleRadius, circleRadius);
         paint->drawText(newPoint->x()-3, newPoint->y()+3, net.at(i)->getId());
    }

    for(int i = 0; i < net.size(); i++)
    {
        //draw line here
        for(int j = 0; j < net.at(i)->get_out_neurons().size(); j++)
        {
            //point from origin neuron
            newPoint->setX(point->x() + net.at(i)->getDrawX() + circleRadius);
            newPoint->setY(point->y() + net.at(i)->getDrawY());

            //point to destonation neuron
            newPoint2->setX(point->x() + net.at(i)->get_out_neurons().at(j)->getDrawX()- circleRadius);
            newPoint2->setY(point->y() + net.at(i)->get_out_neurons().at(j)->getDrawY());

            paint->drawLine(*newPoint, *newPoint2);
        }

    }


    //paint->drawEllipse(*point, 7, 7);
    //paint->drawRect(15,15,100,100);
     ui->label->setPixmap(*pix1); // Added this line




}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------

void MainWindow::defineLevel(std::shared_ptr<neuron_AAC_type_1> neuron)
{
    int max_level = -1;
    int next_element = 0;

    if(neuron->get_in_neurons().empty())
    {
        neuron->setLevel("0");
    }
    else
    {
        for(int i = 0; i < neuron->get_in_neurons().size(); i++)
        {
            if(neuron->get_in_neurons().at(i)->getLevel() == "")
            {
                defineLevel(neuron->get_in_neurons().at(i));
                i--;
            }
            else
            { //?????
                next_element = neuron->get_in_neurons().at(i)->getLevel().toInt();
                if(next_element >= max_level)
                {
                max_level = neuron->get_in_neurons().at(i)->getLevel().toInt();
                }
            }
        }
    }
    neuron->setLevel(QString::number(max_level+1));
}


void MainWindow::defineStructure(QList<std::shared_ptr<neuron_AAC_type_1>> net)
{

    for(int i = 0; i < net.size(); i++) //walk through all the neurons
    {
        //neuron element
         defineLevel(net.at(i));
    }
}



void MainWindow::formGraphVizText(QList<std::shared_ptr<neuron_AAC_type_1>> net)
{
    std::vector<std::shared_ptr<neuron_AAC_type_1>> out_neurons;
    QString filename="neuron_net .txt";
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );

        stream << "graph """ << endl;
        stream << "{" << endl;
        stream << " #   node [fontsize=10,width=«.2», height=«.2», margin=0]; " << endl;
        stream << "#   graph[fontsize=8];" << endl;
        stream << "subgraph fro" << endl;
        stream << "{" << endl;


        for(int i = 0; i < net.size(); i++)
        {
            if(net.at(i)->getTYPE() == "receptor")
            {
                stream << "r" + net.at(i)->getId()+  "[label=r" + net.at(i)->getId() +"_l"+net.at(i)->getLevel() + "][color=red];" << endl;
            }
            else
            {
                stream << "n" + net.at(i)->getId()+  "[label=n" + net.at(i)->getId()  +"_l"+net.at(i)->getLevel() +"][color=blue];" << endl;
            }

            out_neurons = net.at(i)->get_out_neurons();
            for(int j = 0; j < out_neurons.size(); j++)
            {
                if(net.at(i)->getTYPE() == "receptor")
                {
                    stream << "r" + net.at(i)->getId() +" -- "+ "n" +out_neurons.at(j)->getId() << endl;
                }
                else
                {
                    stream << "n" + net.at(i)->getId() +" -- "+ "n" +out_neurons.at(j)->getId() << endl;
                }
            }
        }

        stream << "}" << endl;
        stream << "}" << endl;
    }
}


