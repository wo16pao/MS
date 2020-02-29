#ifndef ADMIN_MODIFYINFO_H
#define ADMIN_MODIFYINFO_H

#include <QWidget>
#include <QTime>
#include <QtSql/QSqlDatabase>

namespace Ui {
class Admin_ModifyInfo;
}

class Admin_ModifyInfo : public QWidget
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

    void mouseClicked();

    void listVisable();

protected:
    void keyPressEvent (QKeyEvent*);

signals:
    void refresh();

private:
    Ui::Admin_ModifyInfo *ui;

    QSqlDatabase db;

    QString m_id;
    QString m_name;
    QTime   m_out;
    QTime   m_in;
};

#endif // ADMIN_MODIFYINFO_H
