#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QDir>
#include <QStringList>
#include <QTimer>
#include <QtNetwork>

#include "../common/common.h"

const QString PATH = "/root/slideshow";

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int t, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStringList * list;
    QTimer * timer;

    QUdpSocket *udpSocket;

    QGst::PipelinePtr m_pipeline;
    QGst::PipelinePtr m_pipeline_audio;

    int count;
    state_t state;
    int time;
    QSize size;

    void open(QString path);
    void action(state_t state_);
    void clear(state_t state_);
    void streamingVideo();
    void streamingCamera();
    void streamingAudio();

private slots:
    void update();
    void processPendingDatagrams();
};

#endif // MAINWINDOW_H
