#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog001.h"
#include "dialog002.h"

#include "widget001.h"

#include <QGuiApplication>
#include <QMutex>
#include <QMouseEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow * MainWindow::m_client = nullptr;
MainWindow *MainWindow::g(QWidget *_parent)
{
    static QMutex mutex;
    if (m_client == nullptr){
        mutex.lock();
        if (m_client == nullptr){
            m_client = new MainWindow(_parent);
            m_client->setVisible( false );
        }
        mutex.unlock();
    }
    else{
        if( _parent != nullptr ){
            m_client->setParent(_parent);
        }
    }
    return m_client;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    qDebug() << event->pos() << event->globalPos();
}


void MainWindow::on_pushButton_clicked()
{
    Dialog001 * dlg1 = new Dialog001(this);
    QInputMethod * pInput = QApplication::inputMethod();
    if( pInput ){
        pInput->reset();
    }
    dlg1->setModal(false);
    //dlg1->setGeometry(0, 0, 1920, 1080);
    dlg1->raise();
    dlg1->show();
}


void MainWindow::on_pushButton_2_clicked()
{
    Dialog002 * dlg2 = new Dialog002(this);
    QInputMethod * pInput = QApplication::inputMethod();
    if( pInput ){
        pInput->reset();
    }
    //dlg2->setGeometry(0, 0, 1920, 1080);
    dlg2->raise();
    dlg2->showFullScreen();
}


void MainWindow::on_pushButton_3_clicked()
{
    Widget001 * wdg001 = new Widget001( this );
    wdg001->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    close();
    qApp->exit(0);
}

