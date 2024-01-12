#ifndef DIALOG001_H
#define DIALOG001_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class Dialog001;
}

class Dialog001 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog001(QWidget *parent = nullptr);
    ~Dialog001();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog001 *ui;

protected:
    void showEvent(QShowEvent *_ev) override;
};

#endif // DIALOG001_H
