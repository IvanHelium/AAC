#ifndef ROBOT_SIMULATOR_H
#define ROBOT_SIMULATOR_H

#include <QObject>
#include <QMap>
#include <QVector>

class robot_simulator : public QObject
{
    Q_OBJECT
public:
    explicit robot_simulator(QObject *parent = nullptr);
    void save_combination(QVector<int> pattern_current, int action_number, QVector<int> pattern_result);
signals:


public slots:


private:
int mass[90][6][90]; // main data structure  pattern of situations -> action -> list of pattern of results



};

#endif // ROBOT_SIMULATOR_H
