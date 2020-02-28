#ifndef ADMIN_ADDDORMITORY_H
#define ADMIN_ADDDORMITORY_H

#include <QWidget>

namespace Ui {
class Admin_AddDormitory;
}

class Admin_AddDormitory : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_AddDormitory(QWidget *parent = nullptr);
    ~Admin_AddDormitory();

public slots:
    void pushButton_back();
    void addDormitory();

signals:
    void refresh();

private:
    Ui::Admin_AddDormitory *ui;
};

#endif // ADMIN_ADDDORMITORY_H
