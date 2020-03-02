#ifndef ADMIN_MODIFYMANAGER_H
#define ADMIN_MODIFYMANAGER_H

#include <QWidget>

namespace Ui {
class Admin_ModifyManager;
}

class Admin_ModifyManager : public QWidget
{
    Q_OBJECT

public:
    explicit Admin_ModifyManager(QWidget *parent = nullptr);
    ~Admin_ModifyManager();

public slots:
    void modifyManager();
    void pushButton_back();

    void getManager(QString ,QString ,QString);

signals:
    void refresh();

private:
    Ui::Admin_ModifyManager *ui;

    QString m_name;
    QString m_id;
    QString m_password;
};

#endif // ADMIN_MODIFYMANAGER_H
