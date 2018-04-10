#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "neuron_aac.h"
#include <QDebug>
#include <QThread>
#include <QSerialPortInfo>
#include <QTimer>
#include <QComboBox>
#include <QScrollBar>

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

    std::shared_ptr<neuron_AAC_type_1> n01(new neuron_AAC_type_1("n01", 3, 3, 0.9, 0.9));

    std::shared_ptr<neuron_AAC_type_1> n02(new neuron_AAC_type_1("n02", 3, 3, 0.9, 0.9));

    std::shared_ptr<neuron_AAC_type_1> n03(new neuron_AAC_type_1("n03", 3, 3, 0.9, 0.9));


    n01->append_out_neurons(n02);
    n02->append_in_neurons(n01);
    n01->append_out_neurons(n03);
    n03->append_in_neurons(n01);


    std::vector <int> FIRST_IMAGE;

    FIRST_IMAGE.push_back(1);
    FIRST_IMAGE.push_back(1);
    FIRST_IMAGE.push_back(1);
    FIRST_IMAGE.push_back(1);
    FIRST_IMAGE.push_back(1);


for(int i = 0; i < 10; i++)
{

    n01->run(FIRST_IMAGE);
    n01->sync();
    n02->run();
    n02->sync();
    n03->run();
    n03->sync();
}






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

    serial->sendDriveForward();
}


//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------

void MainWindow::debugSensorView(uint8_t tact_sensor_data,uint8_t direction_to_resource,uint8_t direction_to_resource_sign, uint8_t distance_to_recource)
{
    qDebug() << "sensor  = " << QString::number(tact_sensor_data) << "direction = " << QString::number(direction_to_resource) << "\n  sign" << QString::number(direction_to_resource_sign)<< "\n  distance" << QString::number(distance_to_recource);
}

