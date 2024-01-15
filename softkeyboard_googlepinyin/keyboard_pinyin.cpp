#include "keyboard_pinyin.h"

#include "softkeyboard_global.h"
#include "define_debug_output.h"

#include <QTimer>

xzKeyboardPinyin* xzKeyboardPinyin::m_client = nullptr;

xzKeyboardPinyin *xzKeyboardPinyin::g(QObject *parent)
{
    if (m_client==nullptr) {
        m_client = new xzKeyboardPinyin(parent);
    }
    return m_client;
}

void xzKeyboardPinyin::set_to_pinyin(bool _py)
{
    if( _py ){
        m_py_input_mode = InputMode::Pinyin;
        m_py_state      = Py_State::Input;
    }
    else{
        m_py_input_mode = InputMode::Latin;
    }
    m_action_type = Action_Type::Unkonow;
}

void xzKeyboardPinyin::s_chooseCandidate(int _id)
{
    m_action_type = Action_Type::Choose_Candidate;
    chooseAndUpdate(_id);
    updateCandidateList();

    emit si_update_input( m_py_composing_str );
}

xzKeyboardPinyin::xzKeyboardPinyin(QObject *parent)
    : QObject{parent}
{
    m_py_input_mode       = InputMode::Latin;//(QPinyinInputMethod::InputMode::Latin),
    m_py_state            = Idle;
    m_py_surface          = "";
    m_py_total_num        = 0;
    m_py_fixed_len        = 0;
    m_py_composing_str    = "";
    m_py_active_cmps_len  = 0;
    m_py_finish_selection = true;
    m_py_pos_del_spl      = -1;
    m_py_is_pos_in_spl    = false;
    m_candidates_list.clear();
    m_pinyin_service      = PinyinDecoderService::getInstance();
    m_pinyin_service->setUserDictionary( true );
    m_action_type = Action_Type::Unkonow;
}


bool xzKeyboardPinyin::py_key_event(Qt::Key key, const QString &text, Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(modifiers)
    QDEBUGT << "key" << key << " text: " << text << " mode " << m_py_input_mode
            << " state " << m_py_state;
    if( m_py_input_mode != InputMode::Pinyin) return false;

    int iTotalNum = m_py_total_num;
    int iState    = m_py_state;
    QList<QString> listCandidates( m_candidates_list );

    bool bReturn = false;
    do{
        if ((key >= Qt::Key_A && key <= Qt::Key_Z) || (key == Qt::Key_Apostrophe)){
            if (m_py_state == Py_State::Predict){
                py_resetToIdleState();
            }
            if (addSpellingChar(text.at(0), m_py_state == Py_State::Idle)){
                m_action_type = Action_Type::Input_Char;
                chooseAndUpdate(-1);
                bReturn = true;
                break;
            }
        }
        else if (key == Qt::Key_Space){
            if (m_py_state != Py_State::Predict && m_py_total_num > 0){
                chooseAndUpdate(0);
                bReturn = true;
                break;
            }
        }
        else if (key == Qt::Key_Return){
            if (m_py_state != Py_State::Predict && m_py_total_num > 0){
                QString surface = m_py_surface;
                py_resetToIdleState();
                // todo:: wait a moment
                //inputContext()->commit(surface);
                bReturn = true;
                break;
            }
        }
        else if (key == Qt::Key_Backspace){
            m_action_type = Action_Type::Remove_Char;
            if(m_py_state == Py_State::Idle){
                //inputContext()->sendKeyClick(key,text,modifiers);
            }
            else if (removeSpellingChar()){
                chooseAndUpdate(-1);
                bReturn = true;
                break;
            }
        }
        else{
            if(m_py_state == Py_State::Input){
                if(text.isEmpty()){
                    chooseAndFinish();
                }
            }
            else if(m_py_state == Py_State::Idle){
                //inputContext()->sendKeyClick(key,text,modifiers);
            }
        }
    }while(false);

    do{
        bool bRefresh = false;
        if( iTotalNum != m_py_total_num) bRefresh = true;
        if( iState    != m_py_state    ) bRefresh = true;
        if( listCandidates != m_candidates_list) bRefresh = true;

        if( !bRefresh ) break;

        updateCandidateList();
        QString sList = QString("");
        for(int ifor0 = 0; ifor0 < m_py_total_num; ++ ifor0 ){
            sList += candidateAt(ifor0) + ",";
        }
        //QDEBUGT << m_py_surface << m_py_composing_str << sList;
    }while(false);
    return bReturn;
}

bool xzKeyboardPinyin::addSpellingChar(QChar _ch, bool _reset)
{
    if (_reset) {
        m_py_surface.clear();
        m_pinyin_service->resetSearch();
    }
    if (_ch == Qt::Key_Apostrophe) {
        if (m_py_surface.isEmpty())
            return false;
        if (m_py_surface.endsWith(_ch))
            return true;
    }
    QDEBUGT << "m_py_surface  >>>> " << m_py_surface;
    m_py_surface.append(_ch);
    QDEBUGT << "m_py_surface  >>>> " << m_py_surface;
    return true;
}

bool xzKeyboardPinyin::removeSpellingChar()
{
    if (m_py_surface.isEmpty())
        return false;
    QVector<int> splStart = m_pinyin_service->spellingStartPositions();
    m_py_is_pos_in_spl = (m_py_surface.length() <= splStart[m_py_fixed_len + 1]);
    m_py_pos_del_spl = m_py_is_pos_in_spl ? m_py_fixed_len - 1 : m_py_surface.length() - 1;
    return true;
}

void xzKeyboardPinyin::chooseAndUpdate(int _candId)
{
    QDEBUGT << " m_py_state " << m_py_state << " id " << _candId;
    //联想状态选择联想词
    if (m_py_state == Predict){
        choosePredictChoice(_candId);
    }
    else{//对输入按键解码
        chooseDecodingCandidate(_candId);
    }

    if (m_py_composing_str.length() > 0){
        QDEBUGT << "id " << _candId << " finish " << m_py_finish_selection
                << " m_py_composing_str " << m_py_composing_str << " len: " << m_py_composing_str.length() << " " << m_py_fixed_len;
        if ((_candId >= 0 || m_py_finish_selection) && m_py_composing_str.length() == m_py_fixed_len){
            QString resultStr = getComposingStrActivePart();
            tryPredict();
            emit si_commit_text( resultStr );
            //InsertTextToCurFocusWt(resultStr);
            //q->inputContext()->commit(resultStr);
        }
        else if (m_py_state == Idle){
            m_py_state = Input;
        }
    }
    else{
        tryPredict();
    }
}

QString xzKeyboardPinyin::candidateAt(int index)
{
    if (index < 0 || index >= m_py_total_num)
        return QString();
    if (index >= m_candidates_list.size()) {
        int fetchMore = qMin(index + 20, m_py_total_num - m_candidates_list.size());
        m_candidates_list.append(m_pinyin_service->fetchCandidates(m_candidates_list.size(), fetchMore, m_py_fixed_len));
        if (index == 0 && m_py_total_num == 1) {
            int surfaceDecodedLen = m_pinyin_service->pinyinStringLength(true);
            if (surfaceDecodedLen < m_py_surface.length())
                m_candidates_list[0] = m_candidates_list[0] + m_py_surface.mid(surfaceDecodedLen).toLower();
        }
    }
    return index < m_candidates_list.size() ? m_candidates_list[index] : QString();
}

bool xzKeyboardPinyin::chooseAndFinish()
{
    if (m_py_state == Predict || m_py_total_num != 0)
        return false;

    chooseAndUpdate(0);
    if (m_py_state != Predict && m_py_total_num > 0)
        chooseAndUpdate(0);

    return true;
}

void xzKeyboardPinyin::chooseDecodingCandidate(int candId)
{
    //Q_ASSERT(m_py_state != Predict);

    int result = 0;
    if (candId == -2 ) {
        if (m_py_surface.length() > 0) {
            result = m_pinyin_service->deleteSearch(m_py_pos_del_spl, m_py_is_pos_in_spl, false);
            QDEBUGT << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << result << m_py_pos_del_spl << m_py_is_pos_in_spl;
            m_py_pos_del_spl = -1;
            m_pinyin_service->resetSearch();

            result = m_pinyin_service->search(m_py_surface);
            QDEBUGT << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << result;
        }
    }
    else if (candId < 0) {
        if (m_py_surface.length() > 0) {
            QDEBUGT << "m_py_surface" << m_py_surface << m_py_pos_del_spl;
            if (m_py_pos_del_spl < 0) {
                result = m_pinyin_service->search(m_py_surface);
            }
            else {
                result = m_pinyin_service->deleteSearch(m_py_pos_del_spl, m_py_is_pos_in_spl, false);
                m_py_pos_del_spl = -1;
            }
            QDEBUGT << "m_py_surface" << m_py_surface << m_py_pos_del_spl;
        }
    }
    else {
        if (m_py_total_num > 1) {
            result = m_pinyin_service->chooceCandidate(candId);
        }
        else {
            QString resultStr;
            if (m_py_total_num == 1) {
                QString undecodedStr = candId < m_candidates_list.length() ? m_candidates_list.at(candId) : QString();
                resultStr = m_pinyin_service->candidateAt(0).mid(0, m_py_fixed_len) + undecodedStr;
            }
            py_resetToIdleState();
            if (!resultStr.isEmpty())
                emit si_commit_text(resultStr);
                //InsertTextToCurFocusWt(resultStr);
                //q->inputContext()->commit(resultStr);
            return;
        }
    }

    resetCandidates();
    m_py_total_num = result;

    m_py_surface = m_pinyin_service->pinyinString(false);
    QVector<int> splStart = m_pinyin_service->spellingStartPositions();
    QString fullSent = m_pinyin_service->candidateAt(0);
    m_py_fixed_len = m_pinyin_service->fixedLength();
    m_py_composing_str = fullSent.mid(0, m_py_fixed_len) + m_py_surface.mid(splStart[m_py_fixed_len + 1]);
    m_py_active_cmps_len = m_py_composing_str.length();

    QDEBUGT << "m_py_surface " << m_py_surface << " len:: " << m_py_surface.length()
            << " fixed_len " << m_py_fixed_len;

    // Prepare the display string.
    QString composingStrDisplay;
    int surfaceDecodedLen = m_pinyin_service->pinyinStringLength(true);
    if (!surfaceDecodedLen) {
        composingStrDisplay = m_py_composing_str.toLower();
        if (!m_py_total_num)
            m_py_total_num = 1;
    }
    else {
        m_py_active_cmps_len = m_py_active_cmps_len - (m_py_surface.length() - surfaceDecodedLen);
        composingStrDisplay = fullSent.mid(0, m_py_fixed_len);
        for (int pos = m_py_fixed_len + 1; pos < splStart.size() - 1; pos++) {
            composingStrDisplay += m_py_surface.mid(splStart[pos], splStart[pos + 1] - splStart[pos]).toUpper();
            if (splStart[pos + 1] < surfaceDecodedLen)
                composingStrDisplay += QLatin1String("'");
        }
        if (surfaceDecodedLen < m_py_surface.length())
            composingStrDisplay += m_py_surface.mid(surfaceDecodedLen).toLower();
    }
    QDEBUGT << " m_py_composing_str " << m_py_composing_str << " composingStrDisplay " << composingStrDisplay << " decode len:" << surfaceDecodedLen
            << "splstrt" << splStart << " " << m_py_surface << m_candidates_list;

    //q->inputContext()->setPreeditText(composingStrDisplay);
    m_py_finish_selection = splStart.size() == (m_py_fixed_len + 2);
    if( surfaceDecodedLen <= 0 ) m_py_finish_selection = false;
    if( m_py_fixed_len <= 0    ) m_py_finish_selection = false;

    if( m_py_finish_selection ){
        if( m_action_type == Action_Type::Remove_Char ){
            QDEBUGT << splStart << " " << m_py_surface << m_candidates_list;
            if( m_py_surface.length() > 0 && m_candidates_list.size() <= 0 ){
                chooseDecodingCandidate( -2 );
            }
        }
    }
    else{
        if( m_action_type == Action_Type::Remove_Char ){
            if( m_py_composing_str != m_py_surface ){
                emit si_update_input( m_py_composing_str );
                QDEBUGT << " si_update_input( m_py_composing_str ); " << m_py_composing_str;
            }
        }
    }
    //m_py_finish_selection = false;
    if (!m_py_finish_selection)
        candidateAt(0);
}

void xzKeyboardPinyin::choosePredictChoice(int choiceId)
{
    //Q_ASSERT(m_py_state == Predict);

    if (choiceId < 0 || choiceId >= m_py_total_num)
        return;

    QString tmp = m_candidates_list.at(choiceId);

    resetCandidates();

    m_candidates_list.append(tmp);
    m_py_total_num = 1;

    m_py_surface.clear();
    m_py_fixed_len = tmp.length();
    m_py_composing_str = tmp;
    m_py_active_cmps_len = m_py_fixed_len;

    m_py_finish_selection = true;
}

QString xzKeyboardPinyin::getComposingStrActivePart()
{
    return m_py_composing_str.mid(0, m_py_active_cmps_len);
}

void xzKeyboardPinyin::resetCandidates()
{
    m_candidates_list.clear();
    m_py_total_num = 0;
    //if (m_py_total_num) {
    //    m_py_total_num = 0;
    //}
    updateCandidateList();
}

void xzKeyboardPinyin::updateCandidateList()
{
    QDEBUGT << m_candidates_list;
    emit si_show_list(m_candidates_list);
}

bool xzKeyboardPinyin::canDoPrediction()
{
    //        Q_Q(QPinyinInputMethod);
    //        QVirtualKeyboardInputContext *inputContext = q->inputContext();
    //        return inputMode == QVirtualKeyboardInputEngine::InputMode::Pinyin &&
    //                m_py_composing_str.length() == m_py_fixed_len &&
    //                inputContext &&
    //                !inputContext->inputMethodHints().testFlag(Qt::ImhNoPredictiveText);

    return false;
}

void xzKeyboardPinyin::tryPredict()
{
    // Try to get the prediction list.
    if (canDoPrediction()) {
        //      Q_Q(QPinyinInputMethod);
        if (m_py_state != Predict)
            py_resetToIdleState();
        //QVirtualKeyboardInputContext *inputContext = q->inputContext();
        //int cursorPosition = inputContext->cursorPosition();
        //int historyStart = qMax(0, cursorPosition - 3);
        //QString history = inputContext->surroundingText().mid(historyStart, cursorPosition - historyStart);
        //m_candidates_list = m_pinyin_service->predictionList(history);
        m_py_total_num = m_candidates_list.size();
        m_py_finish_selection = false;
        m_py_state = Predict;
    }
    else {
        resetCandidates();
    }

    //if (!candidatesCount())
    if( m_py_total_num == 0)
        py_resetToIdleState();
}

void xzKeyboardPinyin::py_resetToIdleState()
{
    if(m_py_state == Idle)
        return;

    m_py_state = Idle;
    m_py_surface.clear();
    m_py_fixed_len = 0;
    m_py_finish_selection = true;
    m_py_composing_str.clear();
    //if (inputContext)
    //    inputContext->setPreeditText(QString());
    m_py_active_cmps_len = 0;
    m_py_pos_del_spl = -1;
    m_py_is_pos_in_spl = false;
    m_py_total_num = 0;
    m_action_type = Action_Type::Unkonow;

    resetCandidates();
}


