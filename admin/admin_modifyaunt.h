#ifndef ADMIN_MODIFYAUNT_H
#define ADMIN_MODIFYAUNT_H

#include <QWidget>
#include "other/basewindow.h"

namespace Ui {
class Admin_ModifyAunt;
}

class Admin_ModifyAunt : public BaseWindow
{
    Q_OBJECT

public:
    explicit Admin_ModifyAunt(QWidget *parent = nullptr);
    ~Admin_ModifyAunt();

public slots:

    void modifyAunt();
    void pushButton_back();

    void getAunt(QString ,QString ,QString,QString);

    void initTitleBar();

signals:
    void refresh();

private:
    Ui::Admin_ModifyAunt *ui;

    QString m_name;
    QString m_id;
    QString m_password;
    QString m_area;
};

#endif // ADMIN_MODIFYAUNT_H
