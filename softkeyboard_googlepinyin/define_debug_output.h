#ifndef DEFINE_DEBUG_OUTPUT_H
#define DEFINE_DEBUG_OUTPUT_H


#include <QDateTime>
#include <QStringList>
#include <QDebug>

#if XZSOFTKEYBOARD_DEBUG == 1
    #define A_FILE      QString(__FILE__).split('/').last()
    #define A_TIME      QDateTime::currentDateTime().toString ("hh:mm:ss")
    #define QDEBUGT     qDebug() << QString("#SKB %1 %2 %3:%4 ").arg(A_TIME).arg(A_FILE).arg(__FUNCTION__).arg(__LINE__)
    #define QDEBUG      qDebug() << QString("#SKB %1:%2 ").arg(A_FILE).arg(__LINE__)
#else
    #define QDEBUGT     QNoDebug()
    #define QDEBUG      QNoDebug()
#endif


#endif // DEFINE_DEBUG_OUTPUT_H
