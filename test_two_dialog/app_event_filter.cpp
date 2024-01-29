#include "app_event_filter.h"

#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>
#include <QCursor>
#include <QWidget>

app_event_filter::app_event_filter(QObject *parent)
    : QObject{parent}
{
    qApp->installEventFilter( this );
    m_pos_last_get = QPoint(-1, -1);
    m_scroll_rate = 2.1f;
}

bool app_event_filter::eventFilter(QObject *_obj, QEvent *_event)
{
    bool isMouseEve = false;
    do{
        if( _event->type() == QEvent::Type::MouseMove           ) { isMouseEve = true; break; }
        //if( _event->type() == QEvent::Type::MouseButtonPress    ) { isMouseEve = true; break; }
        //if( _event->type() == QEvent::Type::MouseButtonRelease  ) { isMouseEve = true; break; }
        //if( _event->type() == QEvent::Type::MouseButtonDblClick ) { isMouseEve = true; break; }
    }while(false);
    do{
        if( ! isMouseEve ) break;
        if( _obj == nullptr ) break;
        if( ! _obj->inherits("QWidget") ) break;

        QWidget * _qwdg = reinterpret_cast<QWidget *>(_obj);
        qDebug() << _obj;
        QMouseEvent * qmeGet = reinterpret_cast<QMouseEvent *>(_event);
        QPoint posGet = _qwdg->mapToGlobal( qmeGet->pos() );
        if( m_pos_last_get.x() == -1 && m_pos_last_get.y() == -1 ){
            m_pos_last_get = posGet;
            break;
        }
        else if( posGet.x() == m_pos_last_get.x() && posGet.y() == m_pos_last_get.y() ){
            break;
        }
        int iChangeX = posGet.x() - m_pos_last_get.x();
        int iChangeY = posGet.y() - m_pos_last_get.y();
        iChangeX = iChangeX * m_scroll_rate;
        iChangeY = iChangeY * m_scroll_rate;

        QPoint qpTar = QPoint(m_pos_last_get.x() + iChangeX, m_pos_last_get.y() + iChangeY);
        m_pos_last_get = qpTar;

        qDebug() << "get pos" << posGet << " tar " << qpTar;
        //QPoint posTar(posGet.x(), posGet.y() * 10);
        //QPoint posTar = m_pos_last_tar;
        QMouseEvent * qmeEv = nullptr;
        qmeEv = new QMouseEvent(QEvent::MouseMove, qpTar, qmeGet->button(), qmeGet->buttons(), Qt::NoModifier);
        //QApplication::postEvent(qApp, qmeEv);
        QApplication::sendEvent(qApp, qmeEv);
        delete qmeEv;

        //QCursor qcCur = qApp.
        //QPoint qpCurO = QCursor::pos();
        //QPoint qpCurO(100, 100);
        QCursor::setPos(qpTar);
        return true;
        //QApplication::postEvent(:sendEvent(&w, &me);
    }while(false);
    return QObject::eventFilter(_obj, _event);

}
