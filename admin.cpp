#include "admin.h"
#include <QtCharts>
QT_CHARTS_USE_NAMESPACE//QtChart名空间
#include "ui_admin.h"
#include <QSqlQuery>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QLineSeries>
#include <QBarSeries>
#include <QDate>
#include <QPointF>



Admin::Admin(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Admin)
{ 
    ui->setupUi(this);
    initTitleBar();
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
    ui->tableWidget->setColumnCount(index);//设置列数
    ui->tableWidget->setHorizontalHeaderLabels(header);//设置标头
    for(int i=0;i<index;++i)
        ui->tableWidget->setColumnWidth(i,160);
    query.exec(get_row);
    if(query.first())
    {
        row=query.value("count(*)").toInt();//获取行数
        ui->tableWidget->setRowCount(row);//设置行数
    }
    if(row)
    {

        //ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //自动调整宽度
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        ui->tableWidget->verticalHeader()->setVisible(false);//隐藏列头
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//只允许单选
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget->setAlternatingRowColors(true);
        query.exec(str);
        //设置内容
        for (int i = 0; query.next(); i++)
        {
            for (int j = 0; j < index; j++)
            {
                if(!flag)
                {
                    ui->tableWidget->setItem(i, j, new QTableWidgetItem(query.value(j).toString()));
                    ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
                }
                else
                {
                    if(j==2||j==3)
                    {
                        QDateTime time;
                        time = query.value(j).toDateTime();
                        QString strBuffer;
                        strBuffer = time.toString("yyyy-MM-dd hh:mm:ss");
                        ui->tableWidget->setItem(i,j,new QTableWidgetItem(strBuffer));
                        ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
                    }
                    else
                    {
                        ui->tableWidget->setItem(i, j, new QTableWidgetItem(query.value(j).toString()));
                        ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
                    }
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
    drawChart();
    drawBarChart();
     ui->label_guide->setText("当前位置：首页");
}

void Admin::pushButton_info()
{
    ui->tabWidget->setCurrentIndex(1);
    ui->label_guide->setText("当前位置：信息管理");
}

void Admin::pushButton_data()
{
    ui->tabWidget->setCurrentIndex(2);
    refresh_label();
     ui->label_guide->setText("当前位置：公告查看");
}

void Admin::pushButton_release()
{
    ui->tabWidget->setCurrentIndex(4);
     ui->label_guide->setText("当前位置：发布公告");
}
//---------------------------------------------------

//查询切换
void Admin::combobox_query(int index)
{
    switch (index) {
    case 0:
        queryUnusual();
        break;
    case 1:
        queryInfo();
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
    case 5:
        queryAunt();
        break;
    case 6:
        queryManager();
        break;
    default:
        break;
    }
}

//--------------各个按钮----------------------------
//搜索按钮
void Admin::pushButton_search()
{
    QString line = ui->lineEdit_search->text();//获取搜索文字
    QString concat = nullptr;
    QString tableName;//表名
    bool flag = false;
    bool case0 = false;
    if(line.isEmpty())//直接搜索不进行搜索
        return;
    //获取表名
    switch (ui->comboBox->currentIndex()) {
    case 0:
        case0 = true;
        tableName = "information";
        concat = "学号,姓名,出门时间,进门时间,时间间隔,体温,是否正常,IFNULL(`备注`,''),";
        flag = true;
        break;
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
    ui->tableWidget->setColumnCount(index);//设置列数
    ui->tableWidget->setHorizontalHeaderLabels(header);//设置标头
    for(int i=0;i<index;++i)
        ui->tableWidget->setColumnWidth(i,160);
    concat.replace(concat.count()-1,1,' ');
    QString get_row = "SELECT count(*) FROM `"+tableName+"` WHERE CONCAT("+concat+") LIKE '%"+line+"%';";
    if(case0)
        get_row = "SELECT count(*) FROM `"+tableName+"` WHERE CONCAT("+concat+") LIKE '%"+line+"%' and 是否正常='不正常';";
    int row=0;
    query.exec(get_row);
    if(query.first())
    {
        row=query.value("count(*)").toInt();//获取行数
        ui->tableWidget->setRowCount(row);//设置行数
    }
    if(row)
    {

        //ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //自动调整宽度
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        ui->tableWidget->verticalHeader()->setVisible(false);//隐藏列头
        ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//只允许单选
        QString str = "SELECT * FROM `"+tableName+"` WHERE CONCAT("+concat+") LIKE '%"+line+"%';";
        if(case0)
            str = "SELECT * FROM `"+tableName+"` WHERE CONCAT("+concat+") LIKE '%"+line+"%' and 是否正常='不正常';";
        query.exec(str);
        //设置内容
        for (int i = 0; query.next(); i++)
        {
            for (int j = 0; j < index; j++)
            {
                if(!flag)
                {
                    ui->tableWidget->setItem(i, j, new QTableWidgetItem(query.value(j).toString()));
                    ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
                }
                else
                {
                    if(j==2||j==3)
                    {
                        QDateTime time;
                        time = query.value(j).toDateTime();
                        QString strBuffer;
                        strBuffer = time.toString("yyyy-MM-dd hh:mm:ss");
                        ui->tableWidget->setItem(i,j,new QTableWidgetItem(strBuffer));
                        ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
                    }
                    else
                    {
                        ui->tableWidget->setItem(i, j, new QTableWidgetItem(query.value(j).toString()));
                        ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
                    }
                }
            }
        }
    }
}

//增加按钮
void Admin::pushButton_add()
{
    switch (ui->comboBox->currentIndex()) {
    case 0:
        connect(m_adm_addInfo,SIGNAL(refresh()),this,SLOT(queryInfo()));
        m_adm_addInfo->show();
        break;
    case 1:
        connect(m_adm_addInfo,SIGNAL(refresh()),this,SLOT(queryInfo()));
        m_adm_addInfo->show();
        break;
    case 2:
        connect(m_adm_addArchive,SIGNAL(refresh()),this,SLOT(queryArchive()));
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
    case 5:
        connect(m_adm_addAunt,SIGNAL(refresh()),this,SLOT(queryAunt()));
        m_adm_addAunt->show();
        break;
    case 6:
        connect(m_adm_addManager,SIGNAL(refresh()),this,SLOT(queryManager()));
        m_adm_addManager->show();
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
    case 0:
        str="delete from `information` where 学号='"+ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text()+"' and 姓名='"+ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()+"' and 出门时间='"+ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text()+"' and 进门时间='"+ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text()+"';";
        break;

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

    case 5:
        str = "delete from `aunt` where 姓名='"+ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text()+"' and 账号='"+ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()+"';";
        break;

    case 6:
        str="delete from `manager` where 姓名='"+ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text()+"' and 账号='"+ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text()+"';";
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
    case 0:
        connect(m_adm_modifyInfo,SIGNAL(refresh()),this,SLOT(queryInfo()));
        m_adm_modifyInfo->getInfo(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text());
        m_adm_modifyInfo->show();
        break;
    case 1:
        connect(m_adm_modifyInfo,SIGNAL(refresh()),this,SLOT(queryInfo()));
        m_adm_modifyInfo->getInfo(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text());
        m_adm_modifyInfo->show();
        break;
    case 2:
        connect(m_adm_modifyArchive,SIGNAL(refresh()),this,SLOT(queryArchive()));
        m_adm_modifyArchive->getArchive(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text());
        m_adm_modifyArchive->show();
        break;
    case 3:
        connect(m_adm_modifyDorm,SIGNAL(refresh()),this,SLOT(queryDormitory()));
        m_adm_modifyDorm->getDormitory(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),4)->text());
        m_adm_modifyDorm->show();
        break;
    case 4:
        connect(m_adm_modifyDean,SIGNAL(refresh()),this,SLOT(queryDean()));
        m_adm_modifyDean->getDean(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text());
        m_adm_modifyDean->show();
        break;
    case 5:
        connect(m_adm_modifyAunt,SIGNAL(refresh()),this,SLOT(queryAunt()));
        m_adm_modifyAunt->getAunt(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),3)->text());
        m_adm_modifyAunt->show();
        break;
    case 6:
        connect(m_adm_modifyManager,SIGNAL(refresh()),this,SLOT(queryManager()));
        m_adm_modifyManager->getManager(ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text(),ui->tableWidget->item(ui->tableWidget->currentRow(),2)->text());
        m_adm_modifyManager->show();
        break;
    default:
        return;
    }
}

//导出按钮
void Admin::exportExcel()
{
    QString filepath=QFileDialog::getSaveFileName(this,tr("Save as"),"datafile",tr(" (*.xlsx);;(*.xls)"));
    if(filepath.isEmpty())
        return;

    exportThread->getFilepath(filepath);
    loading->show();

    QString get_row,str;

    switch (ui->comboBox->currentIndex()) {
    case 1:
        get_row = "SELECT COUNT(*) FROM `information`;";
        str="select * from `information` order by 学号 asc;";
        exportThread->getInfo(get_row,str,"information",true);
        break;
    case 2:
        get_row = "SELECT COUNT(*) FROM `archive`;";//获取行数
        str = "select * from `archive` order by 学号 asc;";//全部数据
        exportThread->getInfo(get_row,str,"archive");
        break;
    case 3:
        get_row = "SELECT COUNT(*) FROM `dormitory`;";
        str = "SELECT * FROM `dormitory` ORDER BY 楼栋号 ASC, 大寝号 asc, 小寝号 asc, 床号 asc;";
        exportThread->getInfo(get_row,str,"dormitory");
        break;
    case 4:
        get_row = "SELECT COUNT(*) FROM `dean`;";
        str = "select * from `dean` order by 班级号 asc;";
        exportThread->getInfo(get_row,str,"dean");
        break;
    case 5:
        get_row = "SELECT COUNT(*) FROM `aunt`;";
        str = "select * from `aunt` order by 宿舍区域 asc;";
        exportThread->getInfo(get_row,str,"aunt");
        break;
    case 6:
        get_row = "SELECT COUNT(*) FROM `manager`;";
        str = "select * from `manager` order by 姓名 asc;";
        exportThread->getInfo(get_row,str,"manager");
        break;
    default:
        return;
    }
    exportThread->start();
}
//导出结束
void Admin::exportExcelFinish()
{
    loading->close();
    exportThread->exit();
    QMessageBox::information(this,"提示","导出完成",QMessageBox::Ok);
}

//导入按钮
void Admin::importExcel()
{
    if(ui->comboBox->currentIndex()<1||ui->comboBox->currentIndex()>6)
        return;
    QString filepath=QFileDialog::getOpenFileName(this,tr("Open"),"",tr(" (*.xlsx);;(*.xls)"));
    if(filepath.isEmpty())
        return;

    importThread->getFilepath(filepath);
    importThread->getIndex(ui->comboBox->currentIndex());
    loading->show();

    importThread->start();
}
//导入结束
void Admin::importExcelFinish(const int &success,const int &failure)
{
    loading->close();
    importThread->exit();
    QString suc = QString::number(success);
    QString fail = QString::number(failure);
    QString str = "导入完成,成功"+suc+"个，失败"+fail+"个";
    QMessageBox::information(this,"提示",str,QMessageBox::Ok);
    switch (ui->comboBox->currentIndex()) {
    case 1:
        queryInfo();
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
    case 5:
        queryAunt();
        break;
    case 6:
        queryManager();
        break;
    }
}

void Admin::readWord()
{
    QString filepath=QFileDialog::getOpenFileName(this,tr("Open"),"",tr(" (*.docx);;(*.doc)"));
    if(filepath.isEmpty())
        return;

    wordThread->getFilepath(filepath);
    loading->show();
    wordThread->start();
}

void Admin::readWordFinish(const QString& text)
{
    loading->close();
    wordThread->exit();
    ui->textEdit_content->setText(text);
}

//公告发布按钮
void Admin::pushButton_release_confirm()
{
    if(ui->lineEdit_subject->text().isEmpty()||ui->lineEdit_title->text().isEmpty()||ui->textEdit_content->toPlainText().isEmpty())
        return;
    if(QMessageBox::No == QMessageBox::information(this,"提示","确定要发布公告吗",QMessageBox::Yes|QMessageBox::No))
        return;

    QString subject = ui->lineEdit_subject->text();
    QString title = ui->lineEdit_title->text();
    QString content = ui->textEdit_content->toPlainText();
    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QSqlQuery query(m_db);
    QString str = "insert into `announcement` values ('"+subject+"','"+title+"','"+date+"','"+content+"');";
    if(query.exec(str))
    {
        ui->label_release_result->setText("公告发布成功");
        ui->lineEdit_subject->clear();
        ui->lineEdit_title->clear();
        ui->textEdit_content->clear();
    }
    else {
        ui->label_release_result->setText("公告发布失败");
    }
    initRelease();
}
//初始化公告内容
void Admin::initRelease()
{
    ui->toolBox->setItemText(0,"");
    ui->textEdit_1->clear();
    ui->toolBox->setItemText(1,"");
    ui->textEdit_2->clear();
    ui->toolBox->setItemText(2,"");
    ui->textEdit_3->clear();
    ui->toolBox->setItemText(3,"");
    ui->textEdit_4->clear();
    ui->toolBox->setItemText(4,"");
    ui->textEdit_5->clear();

    QSqlQuery query(m_db);
    QString str = "select 公告主题,公告标题,公告内容 from `announcement` order by 发布时间 desc;";
    query.exec(str);
    if(query.next())
    {
        ui->toolBox->setItemText(0,query.value(0).toString());
        ui->textEdit_1->setPlainText(query.value(2).toString());
    }
    if(query.next())
    {
        ui->toolBox->setItemText(1,query.value(0).toString());
        ui->textEdit_2->setPlainText(query.value(2).toString());
    }
    if(query.next())
    {
        ui->toolBox->setItemText(2,query.value(0).toString());
        ui->textEdit_3->setPlainText(query.value(2).toString());
    }
    if(query.next())
    {
        ui->toolBox->setItemText(3,query.value(0).toString());
        ui->textEdit_4->setPlainText(query.value(2).toString());
    }
    if(query.next())
    {
        ui->toolBox->setItemText(4,query.value(0).toString());
        ui->textEdit_5->setPlainText(query.value(2).toString());
    }

}

//绘制柱状图
void Admin::drawBarChart()
{
    QBarSeries *series = new QBarSeries();
    QBarSet *set1 = new QBarSet("梅苑");
    QBarSet *set2 = new QBarSet("兰苑");
    QBarSet *set3 = new QBarSet("竹苑");
    QBarSet *set4 = new QBarSet("菊苑");
    QBarSet *set5 = new QBarSet("桃苑");
    QBarSet *set6 = new QBarSet("李苑");
    QBarSet *set7 = new QBarSet("柳苑");
    QBarSet *set8 = new QBarSet("桂苑");
    QBarSet *set9 = new QBarSet("荷苑");

    int max=0;
    QSqlQuery query(m_db);
    QString str = "select count(*) from `information` where 学号 in (select 学号 from `archive` where 宿舍区域='梅苑') and 是否正常='不正常';";
    query.exec(str);
    if(query.next())
    {
        set1->append(query.value(0).toInt());
        if(query.value(0).toInt()>max)
            max = query.value(0).toInt();
    }

    str = "select count(*) from `information` where 学号 in (select 学号 from `archive` where 宿舍区域='兰苑') and 是否正常='不正常';";
    query.exec(str);
    if(query.next())
    {
        set2->append(query.value(0).toInt());
        if(query.value(0).toInt()>max)
            max = query.value(0).toInt();
    }

    str = "select count(*) from `information` where 学号 in (select 学号 from `archive` where 宿舍区域='竹苑') and 是否正常='不正常';";
    query.exec(str);
    if(query.next())
    {
         set3->append(query.value(0).toInt());
        if(query.value(0).toInt()>max)
            max = query.value(0).toInt();
    }

    str = "select count(*) from `information` where 学号 in (select 学号 from `archive` where 宿舍区域='菊苑') and 是否正常='不正常';";
    query.exec(str);
    if(query.next())
    {
         set4->append(query.value(0).toInt());
        if(query.value(0).toInt()>max)
            max = query.value(0).toInt();
    }

    str = "select count(*) from `information` where 学号 in (select 学号 from `archive` where 宿舍区域='桃苑') and 是否正常='不正常';";
    query.exec(str);
    if(query.next())
    {
         set5->append(query.value(0).toInt());
        if(query.value(0).toInt()>max)
            max = query.value(0).toInt();
    }

    str = "select count(*) from `information` where 学号 in (select 学号 from `archive` where 宿舍区域='李苑') and 是否正常='不正常';";
    query.exec(str);
    if(query.next())
    {
         set6->append(query.value(0).toInt());
        if(query.value(0).toInt()>max)
            max = query.value(0).toInt();
    }

    str = "select count(*) from `information` where 学号 in (select 学号 from `archive` where 宿舍区域='柳苑') and 是否正常='不正常';";
    query.exec(str);
    if(query.next())
    {
         set7->append(query.value(0).toInt());
        if(query.value(0).toInt()>max)
            max = query.value(0).toInt();
    }

    str = "select count(*) from `information` where 学号 in (select 学号 from `archive` where 宿舍区域='桂苑') and 是否正常='不正常';";
    query.exec(str);
    if(query.next())
    {
         set8->append(query.value(0).toInt());
        if(query.value(0).toInt()>max)
            max = query.value(0).toInt();
    }

    str = "select count(*) from `information` where 学号 in (select 学号 from `archive` where 宿舍区域='荷苑') and 是否正常='不正常';";
    query.exec(str);
    if(query.next())
    {
        set9->append(query.value(0).toInt());
        if(query.value(0).toInt()>max)
            max = query.value(0).toInt();
    }

    series->append(set1);
    series->append(set2);
    series->append(set3);
    series->append(set4);
    series->append(set5);
    series->append(set6);
    series->append(set7);
    series->append(set8);
    series->append(set9);

    QChart *chart = new QChart();
    chart->addSeries(series);//添加系列到QChart上
    chart->createDefaultAxes();//创建默认轴

    QStringList bottom;
    bottom<<"梅苑"<<"兰苑"<<"竹苑"<<"菊苑"<<"桃苑"<<"李苑"<<"柳苑"<<"桂苑"<<"荷苑";
//    QBarCategoryAxis *axisx = new QBarCategoryAxis();
//    axisx->append(bottom);
//    chart->setAxisX(axisx);
    chart->axes(Qt::Vertical).first()->setRange(0,max);

    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.1f ");
    chart->axisX()->hide();
    chart->legend()->setVisible(true);
    ui->chartWidget_2->setChart(chart);
    //ui->chartWidget_2->chart()->legend()->hide();
}
//绘制图表
void Admin::drawChart()
{
    QLineSeries *series1 = new QLineSeries();//实例化一个QLineSeries对象
    //series1->setName("体温不正常人数");
    series1->setVisible(true);
    //series1->setPointLabelsVisible(true);
    //series1->setPointLabelsFont(QFont("黑体"));
    series1->setPointLabelsFormat("(@xPoint,@yPoint)");
    series1->setPointsVisible(true);

    QList<QPointF> points;
    QSqlQuery query(m_db);
    QDate now = QDateTime::currentDateTime().date();

    QDate ago;
    QString str;
    int max = 0;
    for(int i=0;i<7;++i)
    {
        ago = now.addDays(i-7);
        str = "select count(是否正常) from `information` where 是否正常='不正常' and 进门时间 like '"+ago.toString("yyyy-MM-dd")+"%'";
        query.exec(str);
        if(query.next())
        {
            points.append(QPointF(i+1,query.value(0).toInt()));
            if(query.value(0).toInt()>max)
                max=query.value(0).toInt();
        }
    }


    series1->append(points);


    QChart *chart = new QChart();
    //chart->setLocalizeNumbers(true);//数字是否本地化
    chart->addSeries(series1);//添加系列到QChart上
    chart->createDefaultAxes();//创建默认轴
    //chart->setTitle("体温异常折线图");//设置标题
   // chart->setTitleFont(QFont("黑体"));//设置标题字体
    //chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);//底部对齐
    //chart->legend()->setVisible(true);//设置是否可视
    QStringList bottom;
    bottom<<"7天前"<<"6天前"<<"5天前"<<"4天前"<<"3天前"<<"2天前"<<"1天前";
    QBarCategoryAxis *axisx = new QBarCategoryAxis();
    axisx->append(bottom);
    chart->setAxisX(axisx);
    chart->axes(Qt::Vertical).first()->setRange(0,max);

    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.1f ");

    ui->chartWidget->setChart(chart);
    ui->chartWidget->chart()->legend()->hide();
}
//-----------------------各个按钮-------------------------------------------

//初始化
void Admin::Init()
{
    m_db = QSqlDatabase::database("mysql_connect");
    ui->tabWidget->tabBar()->hide();//隐藏页头
    initRelease();
    drawChart();
    drawBarChart();


    //初始化添加信息类
    m_adm_addDean = new Admin_AddDean;
    m_adm_addInfo = new Admin_AddInfo;
    m_adm_addArchive = new Admin_AddArchive;
    m_adm_addDormitory = new Admin_AddDormitory;
    m_adm_addAunt = new Admin_AddAunt;
    m_adm_addManager = new Admin_AddManager;

    //初始化修改信息类
    m_adm_modifyDean = new Admin_ModifyDean;
    m_adm_modifyDorm = new Admin_ModifyDormitory;
    m_adm_modifyArchive = new Admin_ModifyArchive;
    m_adm_modifyInfo = new Admin_ModifyInfo;
    m_adm_modifyAunt = new Admin_ModifyAunt;
    m_adm_modifyManager = new Admin_ModifyManager;

    //导入导出信息
    exportThread = new ExportThread;
    importThread = new ImportThread;
    wordThread = new WordReadThread;
    loading = new Loading(this);

    m_page = 0;//初始化公告当前页数
    m_page_begin_flag=true;
    m_page_end_flag=false;
    refresh_label();

    QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
        ui->comboBox->setItemDelegate(itemDelegate);

    queryUnusual();
}

//初始化连接
void Admin::InitConnection()
{
    connect(ui->pushButton_back,SIGNAL(clicked()),this,SLOT(pushButton_back()));//返回

    //页面切换
    connect(ui->pushButton_index,SIGNAL(clicked()),this,SLOT(pushButton_index()));
    connect(ui->pushButton_info,SIGNAL(clicked()),this,SLOT(pushButton_info()));
    connect(ui->pushButton_data,SIGNAL(clicked()),this,SLOT(pushButton_data()));
    connect(ui->pushButton_data2,SIGNAL(clicked()),this,SLOT(pushButton_data()));
    connect(ui->pushButton_release,SIGNAL(clicked()),this,SLOT(pushButton_release()));
    connect(ui->pushButton_modify_back,SIGNAL(clicked()),this,SLOT(pushButton_data()));
    connect(ui->pushButton_modify_back_2,SIGNAL(clicked()),this,SLOT(pushButton_data()));
    //connect(ui->pushButton_other,SIGNAL(clicked()),this,SLOT(pushButton_other()));

    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(combobox_query(const int&)));//查询切换

    connect(ui->pushButton_search,SIGNAL(clicked()),this,SLOT(pushButton_search()));//搜素按钮
    connect(ui->pushButton_add,SIGNAL(clicked()),this,SLOT(pushButton_add()));//添加按钮
    connect(ui->pushButton_modify,SIGNAL(clicked()),this,SLOT(pushButton_modify()));//修改按钮
    connect(ui->pushButton_delete,SIGNAL(clicked()),this,SLOT(pushButton_delete()));//删除按钮

    connect(ui->pushButton_export,SIGNAL(clicked()),this,SLOT(exportExcel()));//导出按钮
    connect(ui->pushButton_import,SIGNAL(clicked()),this,SLOT(importExcel()));//导入按钮
    connect(exportThread,SIGNAL(finish()),this,SLOT(exportExcelFinish()));
    connect(importThread,SIGNAL(finish(int,int)),this,SLOT(importExcelFinish(const int&,const int&)));
    connect(ui->pushButton_read_word,SIGNAL(clicked()),this,SLOT(readWord()));
    connect(wordThread,SIGNAL(finish(const QString&)),this,SLOT(readWordFinish(const QString&)));

    connect(ui->pushButton_release_confirm,SIGNAL(clicked()),this,SLOT(pushButton_release_confirm()));//公告发布按钮

    connect(ui->label_title1,SIGNAL(clicked(const QString &)),this,SLOT(label_look(const QString &)));//公告栏查看
    connect(ui->label_title2,SIGNAL(clicked(const QString &)),this,SLOT(label_look(const QString &)));//公告栏查看
    connect(ui->label_title3,SIGNAL(clicked(const QString &)),this,SLOT(label_look(const QString &)));//公告栏查看

    connect(ui->pushButton_next_page,SIGNAL(clicked()),this,SLOT(pushButton_next_page()));//下一页
    connect(ui->pushButton_before_page,SIGNAL(clicked()),this,SLOT(pushButton_before_page()));//上一页
    connect(ui->pushButton_modify_bulletin,SIGNAL(clicked()),this,SLOT(pushButton_bulletin_modify()));//修改公告
    connect(ui->pushButton_bulletin_delete,SIGNAL(clicked()),this,SLOT(pushButton_bulletin_delete()));//删除公告

    connect(ui->pushButton_index,SIGNAL(clicked()),this,SLOT(pushButton_indexStyle()));
    connect(ui->pushButton_info,SIGNAL(clicked()),this,SLOT(pushButton_infoStyle()));
    connect(ui->pushButton_data,SIGNAL(clicked()),this,SLOT(pushButton_dataStyle()));
    connect(ui->pushButton_data2,SIGNAL(clicked()),this,SLOT(pushButton_dataStyle()));
    connect(ui->pushButton_release,SIGNAL(clicked()),this,SLOT(pushButton_releaseStyle()));
}

void Admin::queryUnusual()
{
    QString get_row = "SELECT COUNT(*) FROM `information` where 是否正常='不正常';";
    QString str="select * from `information` where 是否正常='不正常' order by 学号 asc;";
    queryFunction(get_row,str,"information",true);
}

//--------------------------------查询函数--------------------------
//温度信息
void Admin::queryInfo()
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

//宿管信息
void Admin::queryAunt()
{
    QString get_row = "select count(*) from `aunt`;";
    QString str = "select * from `aunt` order by 宿舍区域 asc;";
    queryFunction(get_row,str,"aunt");
}

//管理员信息
void Admin::queryManager()
{
    QString get_row = "select count(*) from `manager`;";
    QString str = "select * from `manager` order by 姓名 asc;";
    queryFunction(get_row,str,"manager");
}

//--------------------------------查询函数--------------------------

//-----------------------------公告信息--------------------------------
void Admin::label_look(const QString &title)
{
    ui->label_release_result_2->clear();
    ui->label_guide->setText("公告修改");
    ui->tabWidget->setCurrentIndex(3);
    ui->lineEdit_look_title->setText(title);
    m_bulletin_title = title;
    QSqlQuery query(m_db);
    QString str = "select 公告主题,公告内容 from `announcement` where 公告标题='"+title+"';";
    query.exec(str);
    if(query.next())
    {
        ui->lineEdit_look_subject->setText(query.value(0).toString());
        ui->textEdit_look_content->setText(query.value(1).toString());
        m_bulletin_content = query.value(1).toString();
    }
}

void Admin::refresh_label()
{
    if(m_page==0)
        m_page_begin_flag=true;
    ui->widget_label_1->setVisible(false);
    ui->widget_label_2->setVisible(false);
    ui->widget_label_3->setVisible(false);
    int num=m_page*3;
    QString str = "select 公告标题 ,公告内容 from `announcement` order by 发布时间 desc;";
    QSqlQuery query(m_db);
    query.exec(str);
    for(int i=0;i<num;++i)//定位到我所需要的地方
        query.next();

    if(query.next())
    {
        ui->widget_label_1->setVisible(true);
        ui->label_title1->setText(query.value(0).toString());
        ui->label_content1->setText(query.value(1).toString());
    }
    else{
        m_page_end_flag=true;
        return;
    }

    if(query.next())
    {
        ui->widget_label_2->setVisible(true);
        ui->label_title2->setText(query.value(0).toString());
        ui->label_content2->setText(query.value(1).toString());
    }
    else{
        m_page_end_flag=true;
        return;
    }

    if(query.next())
    {
        m_page_end_flag=false;
        ui->widget_label_3->setVisible(true);
        ui->label_title3->setText(query.value(0).toString());
        ui->label_content3->setText(query.value(1).toString());
    }
    else{
        m_page_end_flag=true;
        return;
    }
}

void Admin::pushButton_next_page()
{
    if(m_page_end_flag)
        return;
    ui->widget_label_1->setVisible(false);
    ui->widget_label_2->setVisible(false);
    ui->widget_label_3->setVisible(false);
    ++m_page;
    int num=m_page*3;
    QString str = "select 公告标题 ,公告内容 from `announcement` order by 发布时间 desc;";
    QSqlQuery query(m_db);
    query.exec(str);
    for(int i=0;i<num;++i)//定位到我所需要的地方
        query.next();

    if(query.next())
    {
        ui->widget_label_1->setVisible(true);
        ui->label_title1->setText(query.value(0).toString());
        ui->label_content1->setText(query.value(1).toString());
        ui->label_page->setText(QString::number(m_page+1));
    }
    else{
        m_page_end_flag=true;
        pushButton_before_page();
        return;
    }

    if(query.next())
    {
        ui->widget_label_2->setVisible(true);
        ui->label_title2->setText(query.value(0).toString());
        ui->label_content2->setText(query.value(1).toString());
    }
    else{
        m_page_end_flag=true;
        return;
    }

    if(query.next())
    {
        m_page_end_flag = false;
        ui->widget_label_3->setVisible(true);
        ui->label_title3->setText(query.value(0).toString());
        ui->label_content3->setText(query.value(1).toString());
    }
    else{
        m_page_end_flag=true;
        return;
    }

}

void Admin::pushButton_before_page()
{
    if(m_page==0)
        m_page_begin_flag=true;
    if(m_page!=0)
        m_page_begin_flag=false;
    if(m_page_begin_flag)
        return;

    ui->widget_label_1->setVisible(false);
    ui->widget_label_2->setVisible(false);
    ui->widget_label_3->setVisible(false);
    --m_page;
    int num=m_page*3;
    QString str = "select 公告标题 ,公告内容 from `announcement` order by 发布时间 desc;";
    QSqlQuery query(m_db);
    query.exec(str);
    for(int i=0;i<num;++i)//定位到我所需要的地方
        query.next();

    if(query.next())
    {
        ui->widget_label_1->setVisible(true);
        ui->label_title1->setText(query.value(0).toString());
        ui->label_content1->setText(query.value(1).toString());
        ui->label_page->setText(QString::number(m_page+1));
    }
    else{
        m_page_end_flag=true;
        return;
    }

    if(query.next())
    {
        ui->widget_label_2->setVisible(true);
        ui->label_title2->setText(query.value(0).toString());
        ui->label_content2->setText(query.value(1).toString());
    }
    else{
        m_page_end_flag=true;
        return;
    }

    if(query.next())
    {
        m_page_end_flag = false;
        ui->widget_label_3->setVisible(true);
        ui->label_title3->setText(query.value(0).toString());
        ui->label_content3->setText(query.value(1).toString());
    }
    else{
        m_page_end_flag=true;
        return;
    }
}

void Admin::pushButton_bulletin_modify()
{

    if(ui->lineEdit_look_subject->text().isEmpty()||ui->lineEdit_look_title->text().isEmpty()||ui->textEdit_look_content->toPlainText().isEmpty())
        return;
    if(QMessageBox::No==QMessageBox::information(this,"提示","你确定要修改公告吗",QMessageBox::Yes|QMessageBox::No))
        return;

    QString str = "update `announcement` set 公告主题='"+ui->lineEdit_look_subject->text()+"' , 公告标题='"+ui->lineEdit_look_title->text()+"' , 公告内容='"+ui->textEdit_look_content->toPlainText()+"' where 公告标题='"+m_bulletin_title+"' and 公告内容='"+m_bulletin_content+"';";
    QSqlQuery query(m_db);
    if(query.exec(str))
    {
        //QMessageBox::information(this,"提示","修改成功",QMessageBox::Ok);
        ui->label_release_result_2->setText("公告修改成功");
        initRelease();
    }
    else
        ui->label_release_result_2->setText("公告修改失败");
        //QMessageBox::information(this,"提示","修改失败",QMessageBox::Ok);
}

void Admin::pushButton_bulletin_delete()
{
    if(ui->lineEdit_look_subject->text().isEmpty()||ui->lineEdit_look_title->text().isEmpty()||ui->textEdit_look_content->toPlainText().isEmpty())
        return;
    if(QMessageBox::No==QMessageBox::information(this,"提示","你确定要删除公告吗",QMessageBox::Yes|QMessageBox::No))
        return;

    QString str = "delete from `announcement` where 公告标题='"+m_bulletin_title+"' and 公告内容='"+m_bulletin_content+"';";
    QSqlQuery query(m_db);
    if(query.exec(str))
    {
        QMessageBox::information(this,"提示","删除成功",QMessageBox::Ok);
        initRelease();
        pushButton_data();
    }
    else
        QMessageBox::information(this,"提示","删除失败",QMessageBox::Ok);
}
//-----------------------------公告信息--------------------------------

//初始化标题栏
void Admin::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(56,70,85);
    m_titleBar->setTitleIcon(":/icon.png");
    m_titleBar->setTitleContent(QStringLiteral("我的窗口"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}

//设置名字
void Admin::setLabelName(const QString &name)
{
    ui->label_name->setText(name);
}

//---------------------------按钮样式改变--------------------------
void Admin::pushButton_indexStyle()
{
        ui->pushButton_index->setChecked(true);
        ui->pushButton_info->setChecked(false);
        ui->pushButton_data->setChecked(false);
        ui->pushButton_release->setChecked(false);

}

void Admin::pushButton_infoStyle()
{
    ui->pushButton_index->setChecked(false);
    ui->pushButton_info->setChecked(true);
    ui->pushButton_data->setChecked(false);
    ui->pushButton_release->setChecked(false);
}

void Admin::pushButton_dataStyle()
{
    ui->pushButton_index->setChecked(false);
    ui->pushButton_info->setChecked(false);
    ui->pushButton_data->setChecked(true);
    ui->pushButton_release->setChecked(false);
}

void Admin::pushButton_releaseStyle()
{
    ui->pushButton_index->setChecked(false);
    ui->pushButton_info->setChecked(false);
    ui->pushButton_data->setChecked(false);
    ui->pushButton_release->setChecked(true);
}

//---------------------------按钮样式改变--------------------------
void Admin::keyPressEvent(QKeyEvent *event)
{
    if( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
    {
         pushButton_search();
    }
}
