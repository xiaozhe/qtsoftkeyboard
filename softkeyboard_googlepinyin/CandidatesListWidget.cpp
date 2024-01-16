#include "CandidatesListWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QDebug>
#include <QtWidgets>

#include "softkeyboard_global.h"
#include "define_debug_output.h"

CandidatesListWidget::CandidatesListWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StyleSheet, true);
    setAttribute(Qt::WA_StyledBackground, true);

    lastPage = new QPushButton(this);
    nextPage= new QPushButton(this);

    lastPage->setFocusPolicy( Qt::FocusPolicy::NoFocus );
    nextPage->setFocusPolicy( Qt::FocusPolicy::NoFocus );
    this->setFocusPolicy( Qt::FocusPolicy::NoFocus );

    lastPage->setText("<");
    nextPage->setText(">");

    //lastPage->setMinimumSize(50,40);
    //nextPage->setMinimumSize(50,40);

    //nextPage->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    //lastPage->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    connect(lastPage,&QPushButton::clicked,this,&CandidatesListWidget::onLastPage);
    connect(nextPage,&QPushButton::clicked,this,&CandidatesListWidget::onNextPage);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->addWidget(lastPage);
    layout->addWidget(nextPage);
    QDEBUGT << "m_spacer_height" << m_spacer_height;
    m_spacer_item = new QSpacerItem(2,m_spacer_height,QSizePolicy::Expanding,QSizePolicy::Minimum);
    layout->addItem(m_spacer_item);
    layout->setSpacing( 5 );
    layout->setMargin( 0 );

    setLayout(layout);
    //setFont(QFont("Arial",20));

    lastPage->setEnabled(false);
    nextPage->setEnabled(false);
    lastPage->setVisible( false );
    nextPage->setVisible( false );
}

void CandidatesListWidget::setCandidatesList(const QStringList &texts)
{
    QDEBUGT << texts;

    candidatesList = texts;
    pageIndex = 0;
    curIndex = 0;
    headTextIndex = 0;
    tailTextIndex = 0;
    textRects.clear();
    pageHeadIndex.clear();
    pageHeadIndex.append(0);
    if( texts.count() > 0 ){
        lastPage->setVisible( true );
        nextPage->setVisible( true );
    }
    else{
        lastPage->setVisible( false );
        nextPage->setVisible( false );
    }
    update();
}

void CandidatesListWidget::onLastPage()
{
    if(pageIndex == 0)
        return;

    --pageIndex;

    headTextIndex = pageHeadIndex.at(pageIndex);

    if(pageIndex == 0)
        lastPage->setEnabled(false);

    update();
}

void CandidatesListWidget::clear()
{

}

void CandidatesListWidget::setSpacerItemH(int _value)
{
    QDEBUGT << "m_spacer_height" << m_spacer_height;
    m_spacer_height = _value;
    QDEBUGT << "m_spacer_height" << m_spacer_height;
    m_spacer_item->changeSize(2,m_spacer_height,QSizePolicy::Expanding,QSizePolicy::Minimum);
}

void CandidatesListWidget::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
}

void CandidatesListWidget::onNextPage()
{
    if(tailTextIndex >= candidatesList.size() - 1)
        return;

    lastPage->setEnabled(true);

    headTextIndex = tailTextIndex + 1;
    ++pageIndex;

    if(pageIndex >= pageHeadIndex.size())
        pageHeadIndex.append(headTextIndex);

    update();
}

void CandidatesListWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)


    QPainter painter(this);

    //QFont font("Microsoft YaHei",18);
    //painter.setFont(font);

    QRect rectText = nextPage->geometry();
    QDEBUGT << "rectText" << rectText;

    rectText.setLeft(rectText.right() + 20);
    rectText.setRight(width());

    textRects.clear();
    int ifor0;
    for(ifor0 = headTextIndex; ifor0 < candidatesList.size(); ++ifor0){
        if(ifor0 == headTextIndex)
            painter.setPen(QColor(Qt::white));
        else
            painter.setPen(QColor(121,193,59));

        QString strTextDraw = /*QString::number(i - headTextIndex + 1) + "." + */candidatesList[ifor0];
        QRect br = painter.boundingRect(rectText,Qt::AlignLeft | Qt::AlignVCenter,strTextDraw);

        if(br.right() + 10 >= width()){
            nextPage->setEnabled(true);
            break;
        }
        else
            painter.drawText(rectText,Qt::AlignLeft | Qt::AlignVCenter,strTextDraw);

        textRects.append(br);
        tailTextIndex = ifor0;

        rectText.translate(br.width() + 10,0);
    }

    if(ifor0 >= candidatesList.size())
        nextPage->setEnabled(false);
}

void CandidatesListWidget::mousePressEvent(QMouseEvent *e)
{
    for(int i = 0;i < textRects.size();++i) {
        if(textRects[i].contains(e->pos()))
            chooseText(headTextIndex + i);
    }
}
