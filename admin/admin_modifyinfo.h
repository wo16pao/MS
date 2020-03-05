#ifndef ADMIN_MODIFYINFO_H
#define ADMIN_MODIFYINFO_H

#include <QWidget>
#include <QTime>
#include <QtSql/QSqlDatabase>
#include <QListWidgetItem>
#include "other/basewindow.h"

namespace Ui {
class Admin_ModifyInfo;
}

class Admin_ModifyInfo : public BaseWindow
{
    Q_OBJECT

public:
    explicit Admin_ModifyInfo(QWidget *parent = nullptr);
    ~Admin_ModifyInfo();

public slots:

    void modifyInfo();
    void pushButton_back();

    void getInfo(QString ,QString ,QString,QString);
    void initInfo();

    void showId(QString);
    void checkId(QString);

    void mouseClicked(QListWidgetItem*);

    void listVisable();

    void initTitleBar();

protected:
    void keyPressEvent (QKeyEvent*);

signals:
    void refresh();

private:
    Ui::Admin_ModifyInfo *ui;

    QSqlDatabase db;

    QString m_id;
    QString m_name;
    QDateTime   m_out;
    QDateTime   m_in;
};

#endif // ADMIN_MODIFYINFO_H
