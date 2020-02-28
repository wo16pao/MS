#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include <QtSql/QSqlDatabase>



namespace Ui {
class Admin;
}

class Admin : public QWidget
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();

    void queryFunction(const QString &row ,const QString &str,const QString &tableName,bool flag = false);

private:
    void Init();//初始化
    void InitConnection();//绑定控件

    void queryTemperature();//查体温
    void queryArchive();//查学生
    void queryDormitory();//查宿舍
    void queryDean();//查学院

public slots:
    void pushButton_back();

    void pushButton_index();
    void pushButton_info();
    void pushButton_data();
    void pushButton_release();
    void pushButton_other();

    void combobox_query(int);

    void pushButton_search();
    void pushButton_add();
   // void pushButton_delete();
   // void pushButton_modify();

signals:
    void signal_backWindow();//返回上个窗口的信号
    void signal_addClicked();//添加按钮点击信号

private:
    Ui::Admin *ui;

    QSqlDatabase m_db;//连接数据库

};

#endif // ADMIN_H
