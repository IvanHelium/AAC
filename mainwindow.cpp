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


    for(int i = 0; i< 11; i++)
    {
        net1.append(std::shared_ptr<neuron_AAC_type_1> (new neuron_AAC_type_1("n"+std::to_string(i), 3, 3, 0.9, 0.9)));
    }

    int stepX = 50;
    int stepY = 50;
    net1.at(0)->setDrawX(stepX);
    net1.at(0)->setDrawY(stepY);
    net1.at(1)->setDrawX(stepX);
    net1.at(1)->setDrawY(2*stepY);
    net1.at(2)->setDrawX(stepX);
    net1.at(2)->setDrawY(3*stepY);
    net1.at(3)->setDrawX(stepX);
    net1.at(3)->setDrawY(4*stepY);
    net1.at(4)->setDrawX(stepX);
    net1.at(4)->setDrawY(5*stepY);

    net1.at(5)->setDrawX(2*stepX);
    net1.at(5)->setDrawY(stepY + stepY/2);
    net1.at(6)->setDrawX(2*stepX);
    net1.at(6)->setDrawY(2*stepY + stepY/2);
    net1.at(7)->setDrawX(2*stepX);
    net1.at(7)->setDrawY(3*stepY + stepY/2);

    net1.at(8)->setDrawX(3*stepX);
    net1.at(8)->setDrawY(stepY);
    net1.at(9)->setDrawX(3*stepX);
    net1.at(9)->setDrawY(2*stepY);

    net1.at(10)->setDrawX(4*stepX);
    net1.at(10)->setDrawY(2*stepY + stepY/2);



    net1.at(0)->append_out_neurons(net1.at(5));
    net1.at(5)->append_in_neurons(net1.at(0));

    net1.at(1)->append_out_neurons(net1.at(5));
    net1.at(5)->append_in_neurons(net1.at(1));

    net1.at(2)->append_out_neurons(net1.at(8));
    net1.at(8)->append_in_neurons(net1.at(2));

    net1.at(2)->append_out_neurons(net1.at(6));
    net1.at(6)->append_in_neurons(net1.at(2));

    net1.at(3)->append_out_neurons(net1.at(6));
    net1.at(6)->append_in_neurons(net1.at(3));

    net1.at(3)->append_out_neurons(net1.at(7));
    net1.at(7)->append_in_neurons(net1.at(3));

    net1.at(4)->append_out_neurons(net1.at(7));
    net1.at(7)->append_in_neurons(net1.at(4));

    net1.at(5)->append_out_neurons(net1.at(8));
    net1.at(8)->append_in_neurons(net1.at(5));

    net1.at(6)->append_out_neurons(net1.at(9));
    net1.at(9)->append_in_neurons(net1.at(6));

    net1.at(7)->append_out_neurons(net1.at(9));
    net1.at(9)->append_in_neurons(net1.at(7));

    net1.at(8)->append_out_neurons(net1.at(10));
    net1.at(10)->append_in_neurons(net1.at(8));

    net1.at(9)->append_out_neurons(net1.at(10));
    net1.at(10)->append_in_neurons(net1.at(9));



    drawNetOnLabel(net1);

    //n01->append_out_neurons(n02);
    //n02->append_in_neurons(n01);
    //n01->append_out_neurons(n03);
    //n03->append_in_neurons(n01);


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
    paint->setPen(*(new QColor(255,34,255,255)));

    QPoint *point = new QPoint(30, 100);
    QPoint *newPoint = new QPoint(0, 0);
    QPoint *newPoint2 = new QPoint(0, 0);

    for(int i = 0; i < net.size(); i++)
    {
        newPoint->setX(point->x() + net.at(i)->getDrawX());
        newPoint->setY(point->y() + net.at(i)->getDrawY());
         paint->drawEllipse(*newPoint, circleRadius, circleRadius);
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
