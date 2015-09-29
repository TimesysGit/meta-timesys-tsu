#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <stdlib.h>

MainWindow::MainWindow(int t, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    time = t * 3000;
    count = 0;

    ui->setupUi(this);

    QDir dir(PATH);
    list = new QStringList(dir.entryList(QDir::Files));
    timer = new QTimer(this);

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(4000, QUdpSocket::ShareAddress);

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    ui->stackedWidget->setCurrentIndex(0);

/*    for(int i = 0; i < list->size(); i++)
        qDebug() << list->at(i);*/
}

void MainWindow::update()
{
    if(list->size() > 0){
        qDebug() << list->at(count);
        open(list->at(count));
        count++;
        if(!(count < list->size())) count = 0;
    }
}

void MainWindow::open(QString path)
{
    QString str(QString(PATH) + "/" + QString(path));

    QPixmap pic(str);
    size = ui->centralWidget->size();
    ui->picLabel->setPixmap(pic.scaled(size, Qt::KeepAspectRatio));
}

void MainWindow::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << datagram.data();

        state_t stateNew = static_cast<state_t>(atoi(datagram.data()));

        if( stateNew != state){
            clear(state);
            action(stateNew);
        }
        state = stateNew;
    }
}

void MainWindow::action(state_t state_)
{
    switch(state_)
    {
        case SLIDE:
            qDebug() << "slide on";
            ui->stackedWidget->setCurrentIndex(0);
            update();
            timer->start(time);
            break;
 /*       case ALERT:
            qDebug() << "Alert!!!" << QString(PATH);
            timer->stop();ui->stackedWidget->setCurrentIndex(0);
            ui->label->setPixmap(QPixmap(QString(PATH) + "/emergency/image001.png").scaled(size, Qt::KeepAspectRatio));
            break;*/
        case STOP:
            ui->picLabel->setText("");
            break;
        case STREAM_VIDEO:
            streamingVideo();
            break;
        case STREAM_CAMERA:
            streamingCamera();
            break;
        case STREAM_ALERT:
            qDebug() << "Alert!!!" << QString(PATH);
            ui->stackedWidget->setCurrentIndex(0);
            timer->stop();
            ui->picLabel->setPixmap(QPixmap(QString(PATH) + "/../messages/emergency_message.png").scaled(size, Qt::KeepAspectRatio));
            streamingAudio();
            break;
        case STREAM_BELL:
            ui->stackedWidget->setCurrentIndex(0);
            ui->picLabel->setPixmap(QPixmap(QString(PATH) + "/../messages/shift-over.png").scaled(size, Qt::KeepAspectRatio));
            streamingAudio();
            break;
        default:
            break;
    }
}

void MainWindow::clear(state_t state_)
{
    switch(state_)
    {
        case SLIDE:
            timer->stop();
            break;
        case ALERT:
            break;
        case STOP:
            break;
        case STREAM_VIDEO:
        if(!m_pipeline.isNull()){
            qtgst::clearPipeline(m_pipeline);
            ui->stackedWidget->setCurrentIndex(0);
            ui->gstWidget->stopPipelineWatch();
        }
            break;
        case STREAM_CAMERA:
        if(!m_pipeline.isNull()){
            qtgst::clearPipeline(m_pipeline);
            ui->stackedWidget->setCurrentIndex(0);
            ui->gstWidget->stopPipelineWatch();
        }
            break;
        case STREAM_ALERT:
            qtgst::clearPipeline(m_pipeline_audio);
            ui->gstWidget->stopPipelineWatch();
            break;
        case STREAM_BELL:
            qtgst::clearPipeline(m_pipeline_audio);
            ui->gstWidget->stopPipelineWatch();
            break;
        default:
            break;
    }
    ui->picLabel->setPixmap(0);
    ui->picLabel->setText("");
}

void MainWindow::streamingVideo()
{
    qDebug() << "Stream video on";

    qtgst::openPipeline("videosink", m_pipeline);
    ui->gstWidget->watchPipeline(m_pipeline);
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::streamingCamera()
{
    qDebug() << "Stream camera on";

    qtgst::openPipeline("camerasink", m_pipeline);
    ui->gstWidget->watchPipeline(m_pipeline);
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::streamingAudio()
{
    qDebug() << "Stream audio on";

    qtgst::openPipeline("audiosink", m_pipeline_audio);
    ui->gstWidget->watchPipeline(m_pipeline_audio);
    m_pipeline_audio->setState(QGst::StatePlaying);
}

MainWindow::~MainWindow()
{
    m_pipeline->setState(QGst::StateNull);
    ui->gstWidget->stopPipelineWatch();
    m_pipeline.clear();
    delete ui;
}
