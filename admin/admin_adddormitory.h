#ifndef ADMIN_ADDDORMITORY_H
#define ADMIN_ADDDORMITORY_H

#include <QWidget>
#include "other/basewindow.h"

namespace Ui {
class Admin_AddDormitory;
}

class Admin_AddDormitory : public BaseWindow
{
    Q_OBJECT

public:
    explicit Admin_AddDormitory(QWidget *parent = nullptr);
    ~Admin_AddDormitory();

public slots:
    void pushButton_back();
    void addDormitory();

    void initTitleBar();
signals:
    void refresh();

private:
    Ui::Admin_AddDormitory *ui;
};

#endif // ADMIN_ADDDORMITORY_H
