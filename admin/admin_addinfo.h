#ifndef ADMIN_ADDINFO_H
#define ADMIN_ADDINFO_H

#include <QWidget>

namespace Ui {
class Admin_AddInfo;
}

class Admin_AddInfo : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_AddInfo(QWidget *parent = nullptr);
    ~Admin_AddInfo();

private:
    Ui::Admin_AddInfo *ui;
};

#endif // ADMIN_ADDINFO_H
