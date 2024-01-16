#ifndef XZSOFTKEYBOARD_H
#define XZSOFTKEYBOARD_H

#include <QWidget>
#include <QMap>
#include <QPointer>

#include "softkeyboard_global.h"

class QPushButton;
class QSqlDatabase;
class QLineEdit;
class QLabel;

namespace Ui {
class XzSoftKeyboard;
}


class XzSoftKeyboard : public QWidget
{
    Q_OBJECT
private:

public:
    static XzSoftKeyboard *getKeyboard();
    void init(); //"" 表示不使用 汉字词组
    ~XzSoftKeyboard()override;

    void setParentWdg(QWidget * _pw);

    //使用说明，要求输入控件必须是自定义的
    //setTextSlot(QString)  用于更新输入控件内容
    //curTextSignal(QString)  用于获取当前文本控件的内容
    //void setAutoShow(QObject* input_widget);//eg.  setAutoShow(new myLineEdit(this));//控件必须是自定义的


protected:
    void showEvent(QShowEvent *event)override;
    void hideEvent(QHideEvent *event)override;


private:
    Ui::XzSoftKeyboard *ui;
    explicit XzSoftKeyboard(QWidget *parent = nullptr);
    static XzSoftKeyboard* g_keyboard;
    QLineEdit * lb_display_text_=nullptr;


private:
    QWidget* cur_focus_wt_ = nullptr; //记录当前焦点控件
    Qt::WindowFlags m_set_win_flags;

    void init1();  //添加UI中的变量及信号与槽
    void init2();  //布局、数据、等只需要初始化一次的

    void clearBuffer();//清除拼音、等缓存
    void switchLetterStatus();//切换大小写
    QSqlDatabase* db_=nullptr;
    int cur_py_page_=-1;
    QString last_py_{};//上一次保存的可寻找到汉字的拼音组合
    QString cur_py_text_{};
    QString last_py_text_{};//用于确定退格按钮
    //    QLineEdit* out_side_le_=nullptr;
    //    QString cur_out_side_text_="";
    //用于每行的按钮
    enum{row_1, row_2, row_3, row_4, row_5};
    QMap<int, QList<QPushButton*>> btn_list_map_;

    enum{en0,en1,en2,ch0,ch1,ch2};//不同模式下按钮的显示文本，0代表没切换符号时的样子，1，2，3代表不同符号页面，
                                  //需要增加新符号则道init2()中去添加，
    int symbal_page_=en0;
    QMap<int, QStringList>symbol_list_map_={};
    void setSymbolPage(int enNum_chNum);//切换不同符号页

    enum{is_ch_mode,is_capital_mode};
    QMap<int, bool>mode_flag_map_{};

    //    typedef  void (*setText_fp) (const QString&);
    QString objName_="";
    //    QMap<QString, setText_fp>objName_fp_map_;

    void InsertTextToCurFocusWt(const QString& str);
    void DeleteTextFromCurFocusWt();
    void recKeyClicked(const QString& str);//处理所有虚拟键盘的按钮  -------------------------------------------------
private slots:
    void recKeyClickedSlot();//处理所有虚拟键盘的按钮  -------------------------------------------------
    void focusChangedSlot(QWidget *old, QWidget *now);//处理 qApp 过来的内容
protected:
    QWidget * m_top_wdg = nullptr;
protected:
    bool eventFilter(QObject *_obj, QEvent *_event) override;

    //    void show(const QString &curText, const QString &recObjName);
    //    void show(const QString &curText, const QString &recObjName, setText_fp);

    void show(int _x, int _y, QObject * _parent);
public slots:
    void show();
    void hide();
signals:
    void changedOutSideTextSignal(const QString &text, const QString &objName);//表示当前外部输入源文本已经被改变
    void setDefaultColorSignal();


private slots:
    void s_pinyin_commit_text(const QString& _str);
    void s_show_choose_candidate(const QString& _str);

};

#endif // XzSoftKeyboard_H



























