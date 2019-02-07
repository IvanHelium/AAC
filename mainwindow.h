#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QStringListModel>
#include <QLabel>
#include <QJsonObject>
#include <QJsonDocument>
#include <QGlobal.h>
#include "neuron_aac.h"
#include "serialport.h"
#include "fro.h"
#include "neuronknowledgebase.h"
#include "robot_simulator.h"
#include <math.h>



typedef struct SensorData
{
 uint8_t no_sensor;
 uint8_t tactile_forward_sensor;
 uint8_t range_left_sensor;
 uint8_t range_center_sensor;
 uint8_t range_right_sensor;

 uint8_t direction_0_30_left_5;
 uint8_t direction_0_30_right_6;
 uint8_t direction_30_180_left_7;
 uint8_t direction_30_180_right_8;
 uint8_t direction_180_360_left_9;
 uint8_t direction_180_360_right_10;

 uint8_t closer_distance;
 uint8_t same_distance;
 uint8_t longer_distance;

 uint8_t previous_distance;
 uint8_t current_distance;




} SensorData;


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
    void timer_action_update();
    void timer_save_kb_update();
    void debugSensorView(uint8_t tact_sensor_data,uint8_t direction_to_resource,uint8_t direction_to_resource_sign, uint8_t distance_to_recource);

    int randInt(int low, int high);

    void prase_sensor_data(uint8_t tact_sensor_data,uint8_t direction_to_resource,uint8_t direction_to_resource_sign, uint8_t distance_to_recource);


    void drawNetOnLabel(QList<std::shared_ptr<neuron_AAC_type_1>> net);
    void defineStructure(QList<std::shared_ptr<neuron_AAC_type_1>> net);
    void defineLevel(std::shared_ptr<neuron_AAC_type_1> neuron);
    //void link(QList<std::shared_ptr<neuron_AAC_type_1>> net, int form, int to);
    void formGraphVizText(QList<std::shared_ptr<neuron_AAC_type_1>> net);




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

    EvaluationSystem *evaluationSystem;

    SensorData sensordata;

    NeuronKnowledgeBase *neuronKnowledgeBase;

    Robot_simulator *robot_simulator;

    QVector<QVector<int>> input_patterns_list;


};

#endif // MAINWINDOW_H
