#ifndef ROBOT_SIMULATOR_H
#define ROBOT_SIMULATOR_H

#include <QObject>
#include <QMap>
#include <QVector>

#include <QFile>
#include <QTextStream>

class Robot_simulator : public QObject
{
    Q_OBJECT
public:
    explicit Robot_simulator(QObject *parent = nullptr);
    void save_combination(QVector<int> pattern_current, int action_number, QVector<int> pattern_result);

    QVector<QVector<int>> generate_test_data();

    QVector<QVector<int>> get_generated_test_data();

    void save_to_file(QString path);
    void load_to_file(QString path);


signals:


public slots:


private:

//QRandomGenerator

QVector<QVector<int>> input_patterns_list;

int mass[90][6][90]; // main data structure  pattern of situations -> action -> list of pattern of results



};

#endif // ROBOT_SIMULATOR_H
