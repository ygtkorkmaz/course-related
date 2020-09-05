#include <ros/ros.h>
#include <QApplication>
#include <rosThread.h>
#include <QThread>

int main(int argc,char** argv){//do not change this main file
    QApplication app(argc,argv);

    ros::init(argc,argv,"test_project");

    RosThread* rosthread  = new RosThread;//rosthread object is created and its work() function is run here
    QThread* worker = new QThread(&app);
    rosthread->moveToThread(worker);

    QObject::connect(rosthread,SIGNAL(rosFinished()),worker,SLOT(quit()));
    QObject::connect(worker,SIGNAL(finished()),&app,SLOT(quit()));
    QObject::connect(worker,SIGNAL(finished()),rosthread,SLOT(deleteLater()));
    QObject::connect(worker,SIGNAL(started()),rosthread,SLOT(work()));

    worker->start();

    return app.exec();
}
