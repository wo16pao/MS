#ifndef ADMIN_MODIFYDORMITORY_H
#define ADMIN_MODIFYDORMITORY_H

#include <QWidget>
#include "other/basewindow.h"

namespace Ui {
class Admin_ModifyDormitory;
}

class Admin_ModifyDormitory : public BaseWindow
{
    Q_OBJECT

public:
    explicit Admin_ModifyDormitory(QWidget *parent = nullptr);
    ~Admin_ModifyDormitory();

public slots:
    void pushButton_back();
    void modifyDormitory();

    void getDormitory(QString ,QString ,QString,QString,QString);

     void initTitleBar();
signals:
    void refresh();

private:
    Ui::Admin_ModifyDormitory *ui;

    QString m_area;
    QString m_building;
    QString m_big;
    QString m_small;
    QString m_bed;
};

#endif // ADMIN_MODIFYDORMITORY_H
