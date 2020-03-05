#ifndef ADMIN_ADDDEAN_H
#define ADMIN_ADDDEAN_H

#include <QWidget>
#include "other/basewindow.h"

namespace Ui {
class Admin_AddDean;
}

class Admin_AddDean : public BaseWindow
{
    Q_OBJECT

public:
    explicit Admin_AddDean(QWidget *parent = nullptr);
    ~Admin_AddDean();

public slots:
    void addDean();
    void pushButton_back();

    void initTitleBar();

signals:
    void refresh();

private:
    Ui::Admin_AddDean *ui;
};

#endif // ADMIN_ADDDEAN_H
