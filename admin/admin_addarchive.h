#ifndef ADMIN_ADDARCHIVE_H
#define ADMIN_ADDARCHIVE_H

#include <QWidget>

namespace Ui {
class Admin_AddArchive;
}

class Admin_AddArchive : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_AddArchive(QWidget *parent = nullptr);
    ~Admin_AddArchive();

private:
    Ui::Admin_AddArchive *ui;
};

#endif // ADMIN_ADDARCHIVE_H
