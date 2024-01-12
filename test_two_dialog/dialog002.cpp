#include "dialog002.h"
#include "ui_dialog002.h"

Dialog002::Dialog002(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog002)
{
    ui->setupUi(this);
}

Dialog002::~Dialog002()
{
    delete ui;
}
