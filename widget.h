#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

#include "admin.h"
#include "aunt.h"
#include "other/basewindow.h"
#include "other/md5.h"

namespace Ui {
class Widget;
}

class Widget : public BaseWindow
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void SqlConnect();//连接数据库

    void ClearUI();//清理界面

    void timerEvent(QTimerEvent *e);

    void writeInit(const QString &key, const QString &value);
    void readInit(const QString &key, QString &value);

private slots:

    void Login();//登录

    void Reshow();//重现此窗口

    void initTitleBar();

    void animation();

private:
    Ui::Widget *ui;
    QSqlDatabase db;
    Aunt    *m_aunt;//宿管界面
    Admin   *m_adm;//管理员界面

    //动画部分
    QPropertyAnimation *pPosAnimation1;
    QPropertyAnimation *pPosAnimation2;
    QSequentialAnimationGroup *pPosGroup;
    QSequentialAnimationGroup *pPosGroup2;
    QParallelAnimationGroup *group;
    QParallelAnimationGroup *group2;
    int speed1x,speed1y, speed2x,speed2y;

};

#endif // WIDGET_H
