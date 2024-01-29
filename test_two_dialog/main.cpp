#include "mainwindow.h"

#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QTextCodec>
#include <QTime>
#include <QLocale>
#include <QTranslator>

#include <QWidget>
#include <QObject>

#include "dialog001.h"

//#include "inputcontext.h"
#include "softkeyboard.h"

#define SET_WIDGET_Translucent(d)\
(d)->setWindowFlags((d)->windowFlags()|Qt::FramelessWindowHint);\
(d)->setAttribute(Qt::WA_TranslucentBackground, true)

#include "app_event_filter.h"

int main(int argc, char *argv[])
{
    //qputenv("QT_IM_MODULE",QByteArray("Qt5Input"));
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    app_event_filter * aef = new app_event_filter(&a);

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#else
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    auto keyboard = XzSoftKeyboard::getKeyboard();
    keyboard->setGeometry(300, 200, 432, 185);
    //keyboard->init("virtualkeyboard_hasQSQLITE/Resource/ChinesePY.db");
    //keyboard->init("/db/pinyin.db");
    keyboard->init();

//    InputContext* input = new InputContext();
//    a.connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)), input, SLOT(focusChangedSlot(QWidget *, QWidget *)));

    //a.setInputContext(input);
    //a.inputMethod()

    QWidget aMainUI;
    SET_WIDGET_Translucent(&aMainUI);
    QRect rectScreen = QGuiApplication::primaryScreen()->geometry();
    //Anviz::Tools::Wdg::setStyleSheet(&aMainUI, "Dlg_Sw_background");
    aMainUI.setMaximumSize(rectScreen.width(), rectScreen.height());
    aMainUI.setMinimumSize(rectScreen.width(), rectScreen.height());
    aMainUI.resize(rectScreen.size());
    aMainUI.showFullScreen();

    MainWindow::g(&aMainUI)->setGeometry(0, 0, 1920, 1080);
    MainWindow::g(&aMainUI)->showFullScreen();

//    Dialog001 * dlg1 = new Dialog001(&aMainUI);
//    dlg1->setGeometry(0, 0, 1920, 1080);
//    dlg1->raise();
//    dlg1->showFullScreen();

    return a.exec();
}
