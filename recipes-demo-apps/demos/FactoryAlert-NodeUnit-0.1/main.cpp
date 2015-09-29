#include <QApplication>
#include "mainwindow.h"
#include <QSettings>
#include <QDebug>
#include <QGst/Init>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGst::init(&argc, &argv);

    QSettings settings("config.ini", QSettings::IniFormat);
    int time = settings.value("time").toInt();
    if(time == 0) {
        time = 2;
        settings.setValue("time", time);
        settings.sync();
    }
    MainWindow w(time);


    w.showFullScreen();
    a.setOverrideCursor(QCursor(Qt::BlankCursor));
    return a.exec();
}
