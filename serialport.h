#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QList>
#include <QStringList>

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);
    bool openPort(const QString& portName);
    void closePort();

    QStringList getPortNames();


    void sendDriveForward();
    void sendDriveForwardLeft();
    void sendDriveForwardRight();
    void sendDriveBack();
    void sendDriveBackLeft();
    void sendDriveBackRight();

signals:
    void hexReceived(const QByteArray& hexData);
    void sensorDataSignal(uint8_t tact_sensor_data,uint8_t direction_to_resource,uint8_t direction_to_resource_sign, uint8_t distance_to_recource);

public slots:
    void serialReceived();

private:
    void serialSend(uint8_t linear_speed, uint8_t angle_speed, uint8_t angle_speed_direction, uint8_t time);
    void readSensorData(QByteArray Recieved_data);
    uint8_t myCrc(QByteArray Recieved_data);
    uint8_t crcOk(QByteArray Recieved_data, uint8_t recievedCrc);

    QSerialPort serial;
    QByteArray rec_data;
};

#endif // SERIALPORT_H
