#ifndef ADMIN_MODIFYARCHIVE_H
#define ADMIN_MODIFYARCHIVE_H

#include <QWidget>
#include <QtSql/QSqlDatabase>

namespace Ui {
class Admin_ModifyArchive;
}

class Admin_ModifyArchive : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_ModifyArchive(QWidget *parent = nullptr);
    ~Admin_ModifyArchive();

public slots:

    void initConnection();

    void modifyArchive();
    void pushButton_back();

    void getArchive(QString);

    void initArchive(QString);

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
    Ui::Admin_ModifyArchive *ui;

    QString m_id;
    QSqlDatabase db;
};

#endif // ADMIN_MODIFYARCHIVE_H
