#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include "admin.h"
#include "aunt.h"
#include "other/basewindow.h"

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

private slots:

    void Login();//登录

    void Reshow();//重现此窗口

    void initTitleBar();

private:
    Ui::Widget *ui;
    QSqlDatabase db;
    Aunt    *m_aunt;//宿管界面
    Admin   *m_adm;//管理员界面
};

#endif // WIDGET_H
