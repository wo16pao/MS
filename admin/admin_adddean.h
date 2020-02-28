#ifndef ADMIN_ADDDEAN_H
#define ADMIN_ADDDEAN_H

#include <QWidget>

namespace Ui {
class Admin_AddDean;
}

class Admin_AddDean : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_AddDean(QWidget *parent = nullptr);
    ~Admin_AddDean();

public slots:
    void addDean();
    void pushButton_back();

signals:
    void refresh();

private:
    Ui::Admin_AddDean *ui;
};

#endif // ADMIN_ADDDEAN_H
