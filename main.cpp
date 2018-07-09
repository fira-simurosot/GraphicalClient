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
        socket->bind(QHostAddress("192.168.43.105"), 10020, QUdpSocket::ShareAddress);
//        socket->joinMulticastGroup(QHostAddress("224.5.23.2"));
        DataWrapper packet;
        while(runApp) {
            while (socket->hasPendingDatagrams()) {
                QByteArray Buffer;
                Buffer.resize(socket->pendingDatagramSize());
                QHostAddress sender;
                quint16 senderPort;
                socket->readDatagram(Buffer.data(),Buffer.size(),&sender,&senderPort);
                Frame head;
                if (head.ParsePartialFromArray(Buffer.data(), Buffer.size())) {
                    qDebug() << Buffer.size();
                    if(head.has_ball() && head.robots_blue_size() == 5 && head.robots_yellow_size() == 5) {
                        view->updateFrame(head);
                    }
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

