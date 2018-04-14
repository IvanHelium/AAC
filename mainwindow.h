#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QStringListModel>
#include "neuron_aac.h"
#include "serialport.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionConnect_triggered(bool checked);
    void timer_update();
    void debugSensorView(uint8_t tact_sensor_data,uint8_t direction_to_resource,uint8_t direction_to_resource_sign, uint8_t distance_to_recource);

private:
    Ui::MainWindow *ui;
    SerialPort *serial;
    QStringListModel portNamesModel;


    //neuron net
    QList<std::shared_ptr<neuron_AAC_type_1>> net1;
    //std::shared_ptr<neuron_AAC_type_1> n01;
    //std::shared_ptr<neuron_AAC_type_1> n02;
    //std::shared_ptr<neuron_AAC_type_1> n03;
};

#endif // MAINWINDOW_H
