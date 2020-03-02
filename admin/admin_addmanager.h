#ifndef ADMIN_ADDMANAGER_H
#define ADMIN_ADDMANAGER_H

#include <QWidget>

namespace Ui {
class Admin_AddManager;
}

class Admin_AddManager : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_AddManager(QWidget *parent = nullptr);
    ~Admin_AddManager();

public slots:
    void addManager();
    void pushButton_back();

signals:
    void refresh();

private:
    Ui::Admin_AddManager *ui;
};

#endif // ADMIN_ADDMANAGER_H
