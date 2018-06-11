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

    connect(serial, &SerialPort::sensorDataSignal, this, &MainWindow::debugSensorView);



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

    fro_test = new Fro(13);

    for(int i = 0; i < 13; i++)
    {
        fro_test->appendReceptor(std::shared_ptr<neuron_AAC_type_1> (new neuron_AAC_type_1("receptor", QString::number(i),"0", 3, 3, 0.8, 0.9)));

    }

    for(int i = 0; i < 13; i++)
    {
    fro_test->defineMAP(fro_test->getNeuron(i));
    }


    int stepX = 40;
    int stepY = 40;

    for(int i = 0; i < 9; i++)
    {
      net2.append(std::shared_ptr<neuron_AAC_type_1> (new neuron_AAC_type_1("neuron_type_1", QString::number(i),"0", 3, 3, 0.8, 0.9)));
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


    link(net2, 0, 1);
    link(net2, 1, 2);
    link(net2, 3, 1);
    link(net2, 4, 2);
    link(net2, 3, 4);
    link(net2, 4, 5);
    link(net2, 6, 4);
    link(net2, 7, 5);
    link(net2, 6, 7);
    link(net2, 8, 7);

    defineStructure(net2);

    //bool test = fro_test->check_same(net2.at(2)->get_in_neurons(), net2.at(5)->get_in_neurons());

    fro_test->create_new_neuron( QString::number(fro_test->getFroSize()), "2");
    drawNetOnLabel(net2);


    QVector<int> test1 = {1,1,0,0,1};
    QVector<int> test2 = {1,1,0,0};

     bool test = fro_test->vectors_equal(test1,test2);

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


matrixEvent *ME = new matrixEvent(4);

ME->pad();

ME->reset();

ME->update({ 1, 1, 0, 0, 1});




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


void MainWindow::link(QList<std::shared_ptr<neuron_AAC_type_1>> net, int form, int to)
{
    net.at(form)->append_out_neurons(net.at(to));
    net.at(to)->append_in_neurons(net.at(form));
}


