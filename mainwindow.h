#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QStringListModel>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include "neuron_aac.h"
#include "serialport.h"
#include "fro.h"

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

    void drawNetOnLabel(QList<std::shared_ptr<neuron_AAC_type_1>> net);
    void defineStructure(QList<std::shared_ptr<neuron_AAC_type_1>> net);
    void defineLevel(std::shared_ptr<neuron_AAC_type_1> neuron);
    void link(QList<std::shared_ptr<neuron_AAC_type_1>> net, int form, int to);



private:
    Ui::MainWindow *ui;
    SerialPort *serial;
    QStringListModel portNamesModel;


    //neuron net
    //QList<std::shared_ptr<neuron_AAC_type_1>> net1;
    QList<std::shared_ptr<neuron_AAC_type_1>> net2;
    Fro *fro_test;
    QList<std::shared_ptr<neuron_AAC_type_1>> receptors;
    QPixmap pix;



};

#endif // MAINWINDOW_H
