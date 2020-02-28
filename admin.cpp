#include "admin.h"
#include "ui_admin.h"
#include <QtSql/QSqlQuery>
#include <QDebug>
#include <QTime>
#include <QMessageBox>

Admin::Admin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin)
{
    ui->setupUi(this);
    Init();
    InitConnection();

}

Admin::~Admin()
{
    delete ui;
}

//表格功能函数
void Admin::queryFunction(const QString &get_row, const QString &str, const QString &tableName ,bool flag)
{
    ui->tableWidget->clearContents();
    int row=0;
    QSqlQuery query(m_db);
    QStringList header;
    QString head = "show columns from `"+tableName+"`;";
    query.exec(head);
    int index=0;
    while(query.next())
    {
        header << query.value(0).toString();
        index++;
    }
    query.exec(get_row);
    if(query.first())
    {
        row=query.value("count(*)").toInt();//获取行数
        ui->tableWidget->setRowCount(row);//设置行数
    }
    if(row)
    {
        ui->tableWidget->setColumnCount(index);//设置列数
        ui->tableWidget->setHorizontalHeaderLabels(header);//设置标头
        //ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //自动调整宽度
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        ui->tableWidget->verticalHeader()->setVisible(false);//隐藏列头
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//只允许单选
        query.exec(str);
        //设置内容
        for (int i = 0; query.next(); i++)
        {
            for (int j = 0; j < index; j++)
            {
                if(!flag)
                    ui->tableWidget->setItem(i, j, new QTableWidgetItem(query.value(j).toString()));
                else
                {
                    if(j==2||j==3||j==4)
                    {
                        QTime time;
                        time = query.value(j).toTime();
                        QString strBuffer;
                        strBuffer = time.toString("hh:mm:ss");
                        ui->tableWidget->setItem(i,j,new QTableWidgetItem(strBuffer));
                    }
                    else
                        ui->tableWidget->setItem(i, j, new QTableWidgetItem(query.value(j).toString()));
                }
            }
        }
    }
}


void Admin::pushButton_back()
{
    emit signal_backWindow();//返回信号
    this->close();//关闭此窗口
}

//-------------按钮转换页面-------------------------------
void Admin::pushButton_index()
{
    ui->tabWidget->setCurrentIndex(0);
}

void Admin::pushButton_info()
{
    ui->tabWidget->setCurrentIndex(1);
}

void Admin::pushButton_data()
{
    ui->tabWidget->setCurrentIndex(2);
}

void Admin::pushButton_release()
{
    ui->tabWidget->setCurrentIndex(3);
}

void Admin::pushButton_other()
{
    ui->tabWidget->setCurrentIndex(4);
}
//---------------------------------------------------

//查询切换
void Admin::combobox_query(int index)
{
    switch (index) {
    case 1:
        queryTemperature();
        break;
    case 2:
        queryArchive();
        break;
    case 3:
        queryDormitory();
        break;
    case 4:
        queryDean();
        break;
    default:
        break;
    }
}

//搜索按钮
void Admin::pushButton_search()
{
    QString line = ui->lineEdit_search->text();//获取搜索文字
    QString concat = nullptr;
    QString tableName;//表名
    bool flag = false;
    if(line.isEmpty())//直接搜索不进行搜索
        return;
    //获取表名
    switch (ui->comboBox->currentIndex()) {
    case 1:
        tableName = "information";
        concat = "学号,姓名,出门时间,进门时间,时间间隔,体温,是否正常,IFNULL(`备注`,''),";
        flag = true;
        break;
    case 2:
        tableName = "archive";
        break;
    case 3:
        tableName = "dormitory";
        break;
    case 4:
        tableName = "dean";
        break;
    default:
        return;
    }

    ui->tableWidget->clearContents();
    QSqlQuery query(m_db);
    QStringList header;
    QString head = "show columns from `"+tableName+"`;";
    query.exec(head);
    int index=0;
    while(query.next())
    {
        header << query.value(0).toString();
        if(!flag)
            concat += query.value(0).toString() + ",";
        index++;
    }
    concat.replace(concat.count()-1,1,' ');
    QString get_row = "SELECT count(*) FROM `"+tableName+"` WHERE CONCAT("+concat+") LIKE '%"+line+"%';";
    int row=0;
    query.exec(get_row);
    if(query.first())
    {
        row=query.value("count(*)").toInt();//获取行数
        ui->tableWidget->setRowCount(row);//设置行数
    }
    if(row)
    {
        ui->tableWidget->setColumnCount(index);//设置列数
        ui->tableWidget->setHorizontalHeaderLabels(header);//设置标头
        //ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //自动调整宽度
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        ui->tableWidget->verticalHeader()->setVisible(false);//隐藏列头
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//只允许单选
        QString str = "SELECT * FROM `"+tableName+"` WHERE CONCAT("+concat+") LIKE '%"+line+"%';";
        query.exec(str);
        //设置内容
        for (int i = 0; query.next(); i++)
        {
            for (int j = 0; j < index; j++)
            {
                if(!flag)
                    ui->tableWidget->setItem(i, j, new QTableWidgetItem(query.value(j).toString()));
                else
                {
                    if(j==2||j==3||j==4)
                    {
                        QTime time;
                        time = query.value(j).toTime();
                        QString strBuffer;
                        strBuffer = time.toString("hh:mm:ss");
                        ui->tableWidget->setItem(i,j,new QTableWidgetItem(strBuffer));
                    }
                    else
                        ui->tableWidget->setItem(i, j, new QTableWidgetItem(query.value(j).toString()));
                }
            }
        }
    }
}

//增加按钮
void Admin::pushButton_add()
{
    switch (ui->comboBox->currentIndex()) {
    case 1:
        m_adm_addInfo->show();
        break;
    case 2:
        m_adm_addArchive->show();
        break;
    case 3:
        connect(m_adm_addDormitory,SIGNAL(refresh()),this,SLOT(queryDormitory()));
        m_adm_addDormitory->show();
        break;
    case 4:
        connect(m_adm_addDean,SIGNAL(refresh()),this,SLOT(queryDean()));
        m_adm_addDean->show();
        break;
    default:
        return;
    }

}

//删除按钮
void Admin::pushButton_delete()
{
    if(!ui->tableWidget->isItemSelected(ui->tableWidget->currentItem()))
        return;

    if(QMessageBox::No==QMessageBox::question(this,"警告","确定要删除吗",QMessageBox::Yes|QMessageBox::No))
        return;

    QSqlQuery query(m_db);
    QString str;

    switch (ui->comboBox->currentIndex()) {
    case 1:
        str="delete from `information` where 学号='"+ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text()+"' and 姓名='"+ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()+"' and 出门时间='"+ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text()+"' and 进门时间='"+ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text()+"';";
        break;

    case 2:
        str="delete from `archive` where 学号='"+ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text()+"';";
        break;

    case 3:
        str="delete from `dormitory` where 宿舍区域='"+ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text()+"' and 楼栋号='"+ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()+"' and 大寝号='"+ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text()+"' and 小寝号='"+ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text()+"' and 床号='"+ui->tableWidget->item(ui->tableWidget->currentRow(),4)->text()+"';";
        break;

    case 4:
        str="delete from `dean` where 学院名称='"+ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text()+"' and 专业名称='"+ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()+"' and 班级号='"+ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text()+"' and 辅导员='"+ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text()+"';";
        break;

    default:
        return;
    }
    if(query.exec(str))
    {
        combobox_query(ui->comboBox->currentIndex());
        QMessageBox::information(this,"提示","删除成功",QMessageBox::Ok);
    }
    else {
         QMessageBox::critical(this,"提示","删除失败",QMessageBox::Ok);
    }
}

//修改按钮
void Admin::pushButton_modify()
{
    if(!ui->tableWidget->isItemSelected(ui->tableWidget->currentItem()))
        return;

    switch (ui->comboBox->currentIndex()) {
    case 1:

        break;
    case 2:

        break;
    case 3:
        connect(m_adm_modifyDorm,SIGNAL(refresh()),this,SLOT(queryDormitory()));
        m_adm_modifyDorm->getDean(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),4)->text());
        m_adm_modifyDorm->show();
        break;
    case 4:
        connect(m_adm_modifyDean,SIGNAL(refresh()),this,SLOT(queryDean()));
        m_adm_modifyDean->getDean(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text());
        m_adm_modifyDean->show();
        break;
    default:
        return;
    }
}

//初始化
void Admin::Init()
{
    m_db = QSqlDatabase::database("mysql_connect");
    ui->tabWidget->tabBar()->hide();//隐藏页头

    //初始化添加信息类
    m_adm_addDean = new Admin_AddDean;
    m_adm_addInfo = new Admin_AddInfo;
    m_adm_addArchive = new Admin_AddArchive;
    m_adm_addDormitory = new Admin_AddDormitory;

    //初始化修改信息类
    m_adm_modifyDean = new Admin_ModifyDean;
    m_adm_modifyDorm = new Admin_ModifyDormitory;
}

//初始化连接
void Admin::InitConnection()
{
    connect(ui->pushButton_back,SIGNAL(clicked()),this,SLOT(pushButton_back()));//返回

    //页面切换
    connect(ui->pushButton_index,SIGNAL(clicked()),this,SLOT(pushButton_index()));
    connect(ui->pushButton_info,SIGNAL(clicked()),this,SLOT(pushButton_info()));
    connect(ui->pushButton_data,SIGNAL(clicked()),this,SLOT(pushButton_data()));
    connect(ui->pushButton_release,SIGNAL(clicked()),this,SLOT(pushButton_release()));
    connect(ui->pushButton_other,SIGNAL(clicked()),this,SLOT(pushButton_other()));

    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(combobox_query(int)));//查询切换

    connect(ui->pushButton_search,SIGNAL(clicked()),this,SLOT(pushButton_search()));//搜素按钮
    connect(ui->pushButton_add,SIGNAL(clicked()),this,SLOT(pushButton_add()));//添加按钮
    connect(ui->pushButton_modify,SIGNAL(clicked()),this,SLOT(pushButton_modify()));//修改按钮
    connect(ui->pushButton_delete,SIGNAL(clicked()),this,SLOT(pushButton_delete()));//删除按钮
}

//温度信息
void Admin::queryTemperature()
{
    QString get_row = "SELECT COUNT(*) FROM `information`;";
    QString str="select * from `information` order by 学号 asc;";
    queryFunction(get_row,str,"information",true);
}

//学生信息查询
void Admin::queryArchive()
{
    QString get_row = "SELECT COUNT(*) FROM `archive`;";//获取行数
    QString str = "select * from `archive` order by 学号 asc;";//全部数据
    queryFunction(get_row,str,"archive");
}

//宿舍信息
void Admin::queryDormitory()
{
    QString get_row = "SELECT COUNT(*) FROM `dormitory`;";
    QString str = "SELECT * FROM `dormitory` ORDER BY 楼栋号 ASC, 大寝号 asc, 小寝号 asc, 床号 asc;";
    queryFunction(get_row,str,"dormitory");
}

//学院信息
void Admin::queryDean()
{
    QString get_row = "SELECT COUNT(*) FROM `dean`;";
    QString str = "select * from `dean` order by 班级号 asc;";
    queryFunction(get_row,str,"dean");
}
