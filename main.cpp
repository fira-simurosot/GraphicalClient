#include "mainwindow.h"
#include <QApplication>

#include <stdio.h>
#include <QtGui>
#include <QApplication>
#include "soccerview.h"
#include "timer.h"

GLSoccerView *view;

bool runApp = true;

class MyThread : public QThread
{
protected:
  void run()
  {
    static const double minDuration = 0.01; //100FPS
//    RoboCupSSLClient client;
//    client.open(false);
//    SSL_WrapperPacket packet;
    while(runApp) {
//      while (client.receive(packet)) {
//        if (packet.has_detection()) {
//          SSL_DetectionFrame detection = packet.detection();
//          view->updateDetection(detection);
//        }
//        if (packet.has_geometry()) {
//          view->updateFieldGeometry(packet.geometry().field());
//        }
//      }
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

