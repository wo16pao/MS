#ifndef ADMIN_ADDINFO_H
#define ADMIN_ADDINFO_H

#include <QWidget>
#include <QtSql/QSqlDatabase>

namespace Ui {
class Admin_AddInfo;
}

class Admin_AddInfo : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_AddInfo(QWidget *parent = nullptr);
    ~Admin_AddInfo();

public slots:
    void addInfo();
    void pushButton_back();

    void checkId();

signals:
    void refresh();

private:
    Ui::Admin_AddInfo *ui;

    QSqlDatabase db;
};

#endif // ADMIN_ADDINFO_H
