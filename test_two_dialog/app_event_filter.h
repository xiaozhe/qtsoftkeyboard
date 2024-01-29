#ifndef APP_EVENT_FILTER_H
#define APP_EVENT_FILTER_H

#include <QObject>
#include <QPoint>

class app_event_filter : public QObject
{
    Q_OBJECT
public:
    explicit app_event_filter(QObject *parent = nullptr);

protected:
    bool eventFilter(QObject *_obj, QEvent *_event) override;

private:
    QPoint m_pos_last_get;
    double m_scroll_rate;

signals:

};

#endif // APP_EVENT_FILTER_H
