#ifndef WIDGET001_H
#define WIDGET001_H

#include <QWidget>

namespace Ui {
class Widget001;
}

class Widget001 : public QWidget
{
    Q_OBJECT

public:
    explicit Widget001(QWidget *parent = nullptr);
    ~Widget001();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget001 *ui;
};

#endif // WIDGET001_H
