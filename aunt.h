#ifndef AUNT_H
#define AUNT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "other/mylabel.h"

namespace Ui {
class Aunt;
}

class Aunt : public QWidget
{
    Q_OBJECT

public:
    explicit Aunt(QWidget *parent = nullptr);
    ~Aunt();

private:
    void init();
    void initConnection();

public slots:

    void pushButtonBack();//返回按键
    //左边按钮
    void pushButtonIndex();
    void pushButtonInfo();
    void pushButtonBulletin();

    void lable_look(const QString&);//查看公告信息
    void sort_lable(const int& addOrSub);//排序公告
    void pushButton_next_page();//下一页
    void pushButton_before_page();//上一页
    void initRelease();//右侧公告栏

    void setLabelName(const QString&);//设置姓名

    void queryInfo();//查体温
    void queryArchive();//查学生
    void queryDormitory();//查宿舍
    void combobox_query(const int&);//查询切换
    void queryFunction(const QString &get_row ,const QString &str,const QString &tableName,bool flag = false);//查询功能函数

    void setArea(const QString &area);

signals:

    void signal_backWindow();//窗口返回信号

private:
    Ui::Aunt *ui;

    QSqlDatabase m_db;

    int m_page;//定位页数
    bool m_page_flag;//定位页数是否为最后一页

    QString m_bulletin_title;//公告标题
    QString m_bulletin_content;//公告内容

    QString m_area;
};

#endif // AUNT_H
