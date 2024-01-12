#ifndef SOFTKEYBOARD_GLOBAL_H
#define SOFTKEYBOARD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined LIB_SOFTKEYBOARD
    #define XZSKB_LIB_EXPORT Q_DECL_EXPORT
#else
    #define XZSKB_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QDateTime>
#include <QStringList>
#include <QDebug>

#if XZSOFTKEYBOARD_DEBUG == 1
    #define A_FILE      QString(__FILE__).split('/').last()
    #define A_TIME      QDateTime::currentDateTime().toString ("hh:mm:ss")
    #define QDEBUGT     qDebug() << QString("###INPUT %1 %2 %3:%4 ").arg(A_TIME).arg(A_FILE).arg(__FUNCTION__).arg(__LINE__)
    #define QDEBUG      qDebug() << QString("###INPUT %1:%2 ").arg(A_FILE).arg(__LINE__)
#else
    #define QDEBUGT     QNoDebug()
    #define QDEBUG      QNoDebug()
#endif


#endif // SOFTKEYBOARD_GLOBAL_H
