#ifndef AUNT_H
#define AUNT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "other/basewindow.h"
#include "other/mylabel.h"
#include "other/exportthread.h"
#include "other/loading.h"

namespace Ui {
class Aunt;
}

class Aunt : public BaseWindow
{
    Q_OBJECT

public:
    explicit Aunt(QWidget *parent = nullptr);
    ~Aunt();

private:
    void initTitleBar();//初始化标题栏
    void init();//初始化
    void initConnection();//绑定控件

public slots:

    void pushButtonBack();//返回按键
    void pushButton_index();//切换tabwidget按钮
    void pushButton_info();
    void pushButton_data();

    void initRelease();//初始化公告
    void setLabelName(const QString&);//设置姓名
    void setArea(const QString&);//设置宿舍区域

    void label_look(const QString&);//查看公告信息
    void refresh_label();
    void pushButton_next_page();//下一页
    void pushButton_before_page();//上一页

    void queryFunction(const QString &row ,const QString &str,const QString &tableName,bool flag = false);
    void queryUnusual();//查不正常
    void queryInfo();//查体温
    void queryArchive();//查学生
    void queryDormitory();//查宿舍
    void combobox_query(const int&);//查询切换

    void exportExcel();//导出到excel表
    void exportExcelFinish();

    void pushButton_search();//查询
    void keyPressEvent(QKeyEvent *event);

    void getTabIndex(const int&);//切换tab
    void queryIndoor();//最近进门记录
    void queryOutdoor();//最近出门记录
signals:

    void signal_backWindow();//窗口返回信号

private:
    Ui::Aunt *ui;

    QSqlDatabase m_db;
    int m_page;//定位页数
    bool m_page_begin_flag;//定位页数是否是第一页
    bool m_page_end_flag;//定位页数是否为最后一页

    QString m_area;

    ExportThread *exportThread;//导出
    Loading *loading;//等待动画
};

#endif // AUNT_H
