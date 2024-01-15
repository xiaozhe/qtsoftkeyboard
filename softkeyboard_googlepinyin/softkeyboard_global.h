#ifndef SOFTKEYBOARD_GLOBAL_H
#define SOFTKEYBOARD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined LIB_SOFTKEYBOARD
    #define XZSKB_LIB_EXPORT Q_DECL_EXPORT
#else
    #define XZSKB_LIB_EXPORT Q_DECL_IMPORT
#endif

#endif // SOFTKEYBOARD_GLOBAL_H
