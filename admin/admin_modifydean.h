#ifndef ADMIN_MODIFYDEAN_H
#define ADMIN_MODIFYDEAN_H

#include <QWidget>
#include "other/basewindow.h"

namespace Ui {
class Admin_ModifyDean;
}

class Admin_ModifyDean : public BaseWindow
{
    Q_OBJECT

public:
    explicit Admin_ModifyDean(QWidget *parent = nullptr);
    ~Admin_ModifyDean();

public slots:

    void modifyDean();
    void pushButton_back();

    void getDean(QString ,QString ,QString,QString);

    void initTitleBar();
signals:
    void refresh();

private:
    Ui::Admin_ModifyDean *ui;

    QString m_name;
    QString m_major;
    QString m_classId;
    QString m_teacher;
};

#endif // ADMIN_MODIFYDEAN_H
