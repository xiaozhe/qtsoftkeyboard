#include "dialog001.h"
#include "ui_dialog001.h"

#include "mainwindow.h"


#include <QDebug>

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
        qDebug() << "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV";
        qDebug() << "VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV";
        qDebug() << pInput->parent();
        //pInput->setParent( this );
        //this->setInputMethodHints()
    }
}

