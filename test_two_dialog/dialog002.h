#ifndef DIALOG002_H
#define DIALOG002_H

#include <QDialog>

namespace Ui {
class Dialog002;
}

class Dialog002 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog002(QWidget *parent = nullptr);
    ~Dialog002();

private:
    Ui::Dialog002 *ui;
};

#endif // DIALOG002_H
