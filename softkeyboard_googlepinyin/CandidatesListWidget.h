#ifndef CANDIDATELIST_WIDGET_H
#define CANDIDATELIST_WIDGET_H

#include <QWidget>
#include <QSpacerItem>

class QPushButton;
class CandidatesListWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int spacerItemHeight READ SpacerItemH WRITE setSpacerItemH)
public:
    explicit CandidatesListWidget(QWidget *parent = nullptr);

signals:
    void chooseText(int id);
public slots:
    void setCandidatesList(const QStringList & texts);
    void onNextPage();
    void onLastPage();
    void clear();
    void setSpacerItemH(int _value);
    int SpacerItemH(){ return m_spacer_height; };
protected:
    void showEvent(QShowEvent * event) override;
    void paintEvent(QPaintEvent * event) override;
    void mousePressEvent(QMouseEvent *e) override;
private:
    QStringList candidatesList;
    QList<QRect> textRects;
    QList<int>    pageHeadIndex;
    QPushButton * lastPage;
    QPushButton * nextPage;
    QSpacerItem * m_spacer_item;
    int pageIndex = 0;
    int curIndex = 0;
    int headTextIndex = 0;
    int tailTextIndex = 0;
    int m_spacer_height = 0;
};

#endif // TEXTSELECTWIDGET_H
