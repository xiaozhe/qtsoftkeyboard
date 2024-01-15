#ifndef KEYBOARD_PINYIN_H
#define KEYBOARD_PINYIN_H

#include <QObject>
#include <QPointer>

#include "pinyindecoderservice_p.h"

class xzKeyboardPinyin : public QObject
{
    Q_OBJECT
private:
    explicit xzKeyboardPinyin(QObject *parent = nullptr);
    static xzKeyboardPinyin * m_client;
public:
    static xzKeyboardPinyin *g(QObject *parent = nullptr);

public:
    enum Action_Type{
        Input_Char,       //输入一个字符
        Remove_Char,      //删除一个字符
        Choose_Candidate, //选择一个候选
        Unkonow
    };
    enum Py_State{
        Idle,   //
        Input,  //
        Predict //联想状态
    };
    enum InputMode {
        Latin,
        Numeric,
        Dialable,
        Pinyin,
        Cangjie,
        Zhuyin,
        Hangul,
        Hiragana,
        Katakana,
        FullwidthLatin,
        Greek,
        Cyrillic,
        Arabic,
        Hebrew,
        ChineseHandwriting,
        JapaneseHandwriting,
        KoreanHandwriting,
        Thai
    };
private:
    QPointer<PinyinDecoderService> m_pinyin_service;
    QString m_py_surface;
    bool m_py_is_pos_in_spl;
    int m_py_pos_del_spl;
    int m_py_fixed_len;
    int m_py_total_num;
    //int m_py_choices_num;
    QList<QString> m_candidates_list;
    int m_py_state;
    QString m_py_composing_str;
    bool m_py_finish_selection;
    int m_py_active_cmps_len;
    InputMode m_py_input_mode;
    Action_Type m_action_type;

public:
    QString surface() { return m_py_surface;}
    int total_num() { return m_py_total_num; }
    bool isPinyinInput() { return m_py_input_mode == InputMode::Pinyin; }
    void set_to_pinyin(bool _py);

public slots:
    void s_chooseCandidate(int _id);

public:
    bool py_key_event(Qt::Key key, const QString &text, Qt::KeyboardModifiers modifiers);
    bool addSpellingChar(QChar _ch, bool _reset);
    bool removeSpellingChar();
    void chooseAndUpdate(int _candId);

    QString candidateAt(int index);
    bool chooseAndFinish();

    void chooseDecodingCandidate(int candId);
    void choosePredictChoice(int choiceId);
    QString getComposingStrActivePart();
    void resetCandidates();
    void updateCandidateList();
    bool canDoPrediction();
    void tryPredict();
    void py_resetToIdleState();

signals:
    void si_show_list(const QStringList & _strings);
    void si_commit_text(const QString& _str);
    void si_update_input(const QString& _str);

signals:

};

#endif // KEYBOARD_PINYIN_H
