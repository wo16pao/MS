#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include "admin/admin_addarchive.h"
#include "admin/admin_adddean.h"
#include "admin/admin_adddormitory.h"
#include "admin/admin_addinfo.h"
#include "admin/admin_addaunt.h"
#include "admin/admin_addmanager.h"

#include "admin/admin_modifydean.h"
#include "admin/admin_modifydormitory.h"
#include "admin/admin_modifyarchive.h"
#include "admin/admin_modifyinfo.h"
#include "admin/admin_modifyaunt.h"
#include "admin/admin_modifymanager.h"

#include "other/excelexport.h"
#include "other/exportthread.h"
#include "other/excelimport.h"
#include "other/importthread.h"
#include "other/loading.h"
#include "other/mylabel.h"

#include "other/basewindow.h"

namespace Ui {
class Admin;
}

class Admin : public BaseWindow
{
    Q_OBJECT

public:
    explicit Admin(QWidget *parent = nullptr);
    ~Admin();

    void queryFunction(const QString &row ,const QString &str,const QString &tableName,bool flag = false);

private:
    void Init();//初始化
    void InitConnection();//绑定控件

public slots:
    void queryInfo();//查体温
    void queryArchive();//查学生
    void queryDormitory();//查宿舍
    void queryDean();//查学院
    void queryAunt();//查宿管
    void queryManager();//查管理员

public slots:
    void pushButton_back();//返回

    void pushButton_index();//切换tabwidget按钮
    void pushButton_info();
    void pushButton_data();
    void pushButton_release();

    void combobox_query(int);//查询切换

    void pushButton_search();//查询
    void pushButton_add();//添加
    void pushButton_delete();//删除
    void pushButton_modify();//修改

    void exportExcel();//导出到excel表
    void importExcel();//导入excel表到数据库
    void exportExcelFinish();
    void importExcelFinish(const int&,const int&);

    void pushButton_release_confirm();//发布按钮
    void initRelease();//初始化公告

    void drawBarChart();//画柱状图
    void drawChart();//画折线图

    void label_look(const QString&);//查看公告信息
    void refresh_label();
    void pushButton_next_page();//下一页
    void pushButton_before_page();//上一页
    void pushButton_bulletin_modify();//公告修改
    void pushButton_bulletin_delete();//公告删除

    void initTitleBar();//初始化标题栏

    void setLabelName(const QString&);//设置姓名

    void pushButton_indexStyle();
    void pushButton_infoStyle();
    void pushButton_dataStyle();
    void pushButton_releaseStyle();

signals:
    void signal_backWindow();//返回上个窗口的信号


private:
    Ui::Admin *ui;

    QSqlDatabase m_db;//连接数据库

    //添加信息
    Admin_AddDean *m_adm_addDean;
    Admin_AddInfo *m_adm_addInfo;
    Admin_AddArchive *m_adm_addArchive;
    Admin_AddDormitory *m_adm_addDormitory;
    Admin_AddAunt *m_adm_addAunt;
    Admin_AddManager *m_adm_addManager;

    //修改信息
    Admin_ModifyDean *m_adm_modifyDean;
    Admin_ModifyDormitory *m_adm_modifyDorm;
    Admin_ModifyArchive *m_adm_modifyArchive;
    Admin_ModifyInfo *m_adm_modifyInfo;
    Admin_ModifyAunt *m_adm_modifyAunt;
    Admin_ModifyManager *m_adm_modifyManager;

    //导入导出
    ExportThread *exportThread;
    ImportThread *importThread;
    Loading *loading;//等待动画

    int m_page;//定位页数
    bool m_page_begin_flag;//定位页数是否是第一页
    bool m_page_end_flag;//定位页数是否为最后一页

    QString m_bulletin_title;//公告标题
    QString m_bulletin_content;//公告内容

    QFile m_checkQss;
    QFile m_uncheckQss;
    QString m_checked;
    QString m_unchecked;
};

#endif // ADMIN_H
