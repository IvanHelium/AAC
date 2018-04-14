#include "serialport.h"
#include <QDebug>
#include <QSerialPortInfo>

#define FIXED_HEADER 0xC4


uint8_t serialDataState = 0;
uint8_t serialDataOldState = 0;

SerialPort::SerialPort(QObject *parent) :
    QObject(parent),
    serial(this)
{


    connect(&serial, SIGNAL(readyRead()), this, SLOT(serialReceived())); //connect singal readyread to function serial_Received
}


bool SerialPort::openPort(const QString &portName)
{

    serial.setPortName(portName);
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setParity(QSerialPort::NoParity);
    serial.setDataBits(QSerialPort::Data8);

    if (serial.open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened";
        qDebug() << QString::number( serial.QSerialPort::baudRate(QSerialPort::AllDirections));
        return true;
    }
    qDebug() << "Could not open serial port";
    return false;
}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void SerialPort::closePort()
{
    if (serial.isOpen())
        serial.close();
}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------


QStringList SerialPort::getPortNames()
{
    QStringList list;
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        list.append(serialPortInfo.portName());
    }
    return list;
}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void SerialPort::serialReceived()
{

    QByteArray Recieved_data;

    uint8_t fixedHeader =  0x00;//FIXED_HEADER;



    if(serial.bytesAvailable() > 0)
    {
        Recieved_data.clear();
        Recieved_data = serial.readAll();

        for(int i = 0; i < Recieved_data.size(); i++)
        {
            if(serialDataOldState == 7 && serialDataState == 0)
            {
                serialDataOldState=0;

                break;
            }
            switch(serialDataState)
            {
                case 0:
                    if(Recieved_data[i] == FIXED_HEADER)
                    {
                        rec_data.clear();
                        rec_data.append(Recieved_data[i]);
                        serialDataState = 1;
                    }
                break;
                case 1:
                        rec_data.append(Recieved_data[i]);
                        serialDataState = 2;
                break;
                case 2:
                        rec_data.append(Recieved_data[i]);
                        serialDataState = 3;
                break;
                case 3:
                        rec_data.append(Recieved_data[i]);
                        serialDataState = 4;
                break;
                case 4:
                        rec_data.append(Recieved_data[i]);
                        serialDataState = 5;
                break;
                case 5:
                        rec_data.append(Recieved_data[i]);
                        serialDataState = 6;
                break;
                case 6:
                        rec_data.append(Recieved_data[i]);
                        serialDataState = 7;
                break;
                case 7:
                        rec_data.append(Recieved_data[i]);
                        serialDataState = 0;
                        serialDataOldState = 7;
                break;

           }
        }
    }


    readSensorData(rec_data);
    rec_data.clear();
    Recieved_data.clear();


    emit hexReceived(rec_data.toHex());
    //processData(Recieved_data);
    //qDebug() << Recieved_data.toHex();
}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void SerialPort::serialSend(uint8_t linear_speed, uint8_t angle_speed, uint8_t angle_speed_direction, uint8_t time)
{
    uint8_t fixedHeader = 0xC3;
    QByteArray mainTargetRequest;

    mainTargetRequest.append(static_cast<char>(fixedHeader));     //check here
    mainTargetRequest.append(static_cast<char>(linear_speed));
    mainTargetRequest.append(static_cast<char>(angle_speed));
    mainTargetRequest.append(static_cast<char>(angle_speed_direction));
    mainTargetRequest.append(static_cast<char>(time));
    serial.write(mainTargetRequest);

}


void SerialPort::readSensorData(QByteArray Recieved_data)
{
    uint8_t tact_sensor_data;
    uint8_t direction_to_resource;
    uint8_t direction_to_resource_sign;
    uint8_t distance_to_recource;
    uint8_t crc;
    crc = Recieved_data[7];
    if(Recieved_data[0] == FIXED_HEADER)
    {
        if(crcOk(Recieved_data,crc))
        {
            tact_sensor_data = Recieved_data[1];
            direction_to_resource = Recieved_data[2];
            direction_to_resource_sign = Recieved_data[3];
            distance_to_recource = Recieved_data[4];

            emit sensorDataSignal(tact_sensor_data, direction_to_resource, direction_to_resource_sign, distance_to_recource);
        }
    }
}

//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
uint8_t SerialPort::myCrc(QByteArray Recieved_data)
{
    uint8_t crc= 0x00;
    crc = Recieved_data[0];
    for(int i = 1; i < Recieved_data.size()-1; i++)
    {
        crc = crc ^ Recieved_data[i];
    }

    return crc;
}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------

uint8_t SerialPort::crcOk(QByteArray Recieved_data, uint8_t recievedCrc)
{
    uint8_t calculatedCrc;
    calculatedCrc = myCrc(Recieved_data);
    if(recievedCrc == calculatedCrc)
    {
        return 1;
    }
    return 0;
}

//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------

void SerialPort::sendDriveForward()
{
    serialSend(0xFE, 0x00, 0x00, 0x14);
}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void SerialPort::sendDriveForwardLeft()
{
    serialSend(0xCE, 0xAF, 0x01, 0x14);
}

//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void SerialPort::sendDriveForwardRight()
{
    serialSend(0xCE, 0xAF, 0x00, 0x14);
}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void SerialPort::sendDriveBack()
{
    serialSend(0x7E, 0x00, 0x00, 0x14);
}
//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void SerialPort::sendDriveBackLeft()
{
    serialSend(0x4E, 0xAF, 0x00, 0x14);
}

//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------
void SerialPort::sendDriveBackRight()
{
    serialSend(0x4E, 0xAF, 0x01, 0x14);
}

//---------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------






