#ifndef ADMIN_H
#define ADMIN_H

#include <QWidget>
#include <QtSql/QSqlDatabase>
#include "admin/admin_addarchive.h"
#include "admin/admin_adddean.h"
#include "admin/admin_adddormitory.h"
#include "admin/admin_addinfo.h"
#include "admin/admin_modifydean.h"
#include "admin/admin_modifydormitory.h"
#include "admin/admin_modifyarchive.h"
#include "admin/admin_modifyinfo.h"

#include "other/excelexport.h"
#include "other/exportthread.h"
#include "other/excelimport.h"
#include "other/importthread.h"
#include "other/loading.h"

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

public slots:
    void queryInfo();//查体温
    void queryArchive();//查学生
    void queryDormitory();//查宿舍
    void queryDean();//查学院

public slots:
    void pushButton_back();//返回

    void pushButton_index();//切换tabwidget按钮
    void pushButton_info();
    void pushButton_data();
    void pushButton_release();
    void pushButton_other();

    void combobox_query(int);//查询切换

    void pushButton_search();//查询
    void pushButton_add();//添加
    void pushButton_delete();//删除
    void pushButton_modify();//修改

    void exportExcel();//导出到excel表
    void importExcel();//导入excel表到数据库
    void exportExcelFinish();
    void importExcelFinish(int,int);

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

    //修改信息
    Admin_ModifyDean *m_adm_modifyDean;
    Admin_ModifyDormitory *m_adm_modifyDorm;
    Admin_ModifyArchive *m_adm_modifyArchive;
    Admin_ModifyInfo *m_adm_modifyInfo;

    //导入导出
    ExportThread *exportThread;
    ImportThread *importThread;
    Loading *loading;//等待动画
};

#endif // ADMIN_H
