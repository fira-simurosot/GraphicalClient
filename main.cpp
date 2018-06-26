#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>
#include <QtGui>
#include <QApplication>
#include "soccerview.h"
#include "timer.h"
#include <QtNetwork>
#include <QHostAddress>
#include <messages_parsian_simurosot_data_wrapper.pb.h>

GLSoccerView *view;

bool runApp = true;

class MyThread : public QThread
{
protected:
    void run()
    {
        static const double minDuration = 0.01; //100FPS
        QUdpSocket* socket = new QUdpSocket();
        socket->bind(QHostAddress::AnyIPv4, 10020, QUdpSocket::ShareAddress);
        socket->joinMulticastGroup(QHostAddress("224.5.23.2"));
        DataWrapper packet;
        while(runApp) {
            while (socket->hasPendingDatagrams()) {
                QByteArray Buffer;
                Buffer.resize(socket->pendingDatagramSize());
                QHostAddress sender;
                quint16 senderPort;
                socket->readDatagram(Buffer.data(),Buffer.size(),&sender,&senderPort);
                qDebug() << Buffer.data() << senderPort << sender.toString();
                packet.ParseFromArray(Buffer.data(), Buffer.size());
                view->updatePacket(packet);
            }
            Sleep(minDuration);
        }
    }

public:
    MyThread(QObject* parent = 0){}
    ~MyThread(){}
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    view = new GLSoccerView();
    view->show();
    MyThread thread;
    thread.start();
    int retVal = app.exec();
    runApp = false;
    thread.wait();
    return retVal;
}

