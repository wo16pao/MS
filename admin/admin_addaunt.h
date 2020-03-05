#ifndef ADMIN_ADDAUNT_H
#define ADMIN_ADDAUNT_H

#include <QWidget>
#include "other/basewindow.h"

namespace Ui {
class Admin_AddAunt;
}

class Admin_AddAunt : public BaseWindow
{
    Q_OBJECT

public:
    explicit Admin_AddAunt(QWidget *parent = nullptr);
    ~Admin_AddAunt();

public slots:
    void addAunt();
    void pushButton_back();

    void initTitleBar();

signals:
    void refresh();

private:
    Ui::Admin_AddAunt *ui;
};

#endif // ADMIN_ADDAUNT_H
