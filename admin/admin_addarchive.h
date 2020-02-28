#ifndef ADMIN_ADDARCHIVE_H
#define ADMIN_ADDARCHIVE_H

#include <QWidget>
#include <QtSql/QSqlDatabase>


namespace Ui {
class Admin_AddArchive;
}

class Admin_AddArchive : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_AddArchive(QWidget *parent = nullptr);
    ~Admin_AddArchive();

    void initConnection();//初始化连接


public slots:
    void addArchive();
    void pushButton_back();

    void initDean();//初始化学院名称
    void initDean2(QString);
    void initDean3(QString);
    void initDean4(QString);

    void initDorm();//初始化宿舍名称
    void initDorm2(QString);
    void initDorm3(QString);
    void initDorm4(QString);
    void initDorm5(QString);

    void initCombobox();

signals:
    void refresh();


private:
    Ui::Admin_AddArchive *ui;

    QSqlDatabase db;

};

#endif // ADMIN_ADDARCHIVE_H
