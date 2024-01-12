#include "widget001.h"
#include "ui_widget001.h"

Widget001::Widget001(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget001)
{
    ui->setupUi(this);
}

Widget001::~Widget001()
{
    delete ui;
}

void Widget001::on_pushButton_clicked()
{
    close();
}

