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
        QUdpSocket* socket = new QUdpSocket();
        socket->bind(QHostAddress("172.21.224.130"), 10030, QUdpSocket::ShareAddress);
        while(runApp) {
            while (socket->hasPendingDatagrams()) {
                QByteArray Buffer;
                Buffer.resize(socket->pendingDatagramSize());
                QHostAddress sender;
                quint16 senderPort;
                socket->readDatagram(Buffer.data(),Buffer.size(),&sender,&senderPort);
                DataWrapper packet;
                if (packet.ParsePartialFromArray(Buffer.data(), Buffer.size())) {
                    view->updatePacket(packet);
                } else {
                    qDebug() << Buffer.data() << "FAILED TO PARSE";
                }
            }
        }
    }

public:
    MyThread(QObject* parent = 0){}
    ~MyThread(){}
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    view = new GLSoccerView();
    view->show();
    MyThread thread;
    thread.start();
    int retVal = app.exec();
    runApp = false;
    thread.wait();
    return retVal;
}

