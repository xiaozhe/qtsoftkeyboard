#include "dialog001.h"
#include "ui_dialog001.h"

#include "mainwindow.h"


#include <QDebug>

#include <QDateTime>
#include <QStringList>
#include <QDebug>

    #define A_FILE      QString(__FILE__).split('/').last()
    #define A_TIME      QDateTime::currentDateTime().toString ("hh:mm:ss")
    #define QDEBUGT     qDebug() << QString("#SKB %1 %2 %3:%4 ").arg(A_TIME).arg(A_FILE).arg(__FUNCTION__).arg(__LINE__)
    #define QDEBUG      qDebug() << QString("#SKB %1:%2 ").arg(A_FILE).arg(__LINE__)

Dialog001::Dialog001(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog001)
{
    ui->setupUi(this);
}

Dialog001::~Dialog001()
{
    delete ui;
}

void Dialog001::on_pushButton_clicked()
{
    MainWindow::g()->showFullScreen();
    close();
}

void Dialog001::showEvent(QShowEvent *_ev)
{
    //VirtualKeyBoard::getKeyboard()->setParentWdg( this );

    QInputMethod * pInput = QApplication::inputMethod();
    if( pInput ){
        QDEBUGT << "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV";
        QDEBUGT << "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV";
        QDEBUGT << pInput->parent();
        //pInput->setParent( this );
        //this->setInputMethodHints()
    }
}

