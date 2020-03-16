#include "aunt.h"
#include "ui_aunt.h"
#include <QDateTime>
#include <QSqlQuery>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QDate>
#include <QPointF>
#include <QStyledItemDelegate>

Aunt::Aunt(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Aunt)
{
    ui->setupUi(this);
    initTitleBar();
    init();
    initConnection();

}

Aunt::~Aunt()
{
    delete ui;
}

void Aunt::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(56,70,85);
    m_titleBar->setTitleIcon(":/icon.png");
    m_titleBar->setTitleContent(QStringLiteral("我的窗口"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}

//初始化
void Aunt::init()
{
    m_db = QSqlDatabase::database("mysql_connect");
    ui->tabWidget->tabBar()->hide();//隐藏页头
    initRelease();

    m_page = 0;//初始化公告当前页数
    m_page_begin_flag=true;
    m_page_end_flag=false;
    refresh_label();
    QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
    ui->comboBox->setItemDelegate(itemDelegate);
    exportThread = new ExportThread;
    loading = new Loading(this);

}
//初始化连接
void Aunt::initConnection()
{
    connect(ui->pushButton_back,SIGNAL(clicked()),this,SLOT(pushButtonBack()));//返回按钮

    //页面切换
    connect(ui->pushButton_index,SIGNAL(clicked()),this,SLOT(pushButton_index()));
    connect(ui->pushButton_info,SIGNAL(clicked()),this,SLOT(pushButton_info()));
    connect(ui->pushButton_data,SIGNAL(clicked()),this,SLOT(pushButton_data()));
    connect(ui->pushButton_data2,SIGNAL(clicked()),this,SLOT(pushButton_data()));

    connect(ui->label_title1,SIGNAL(clicked(const QString &)),this,SLOT(label_look(const QString &)));//公告栏查看
    connect(ui->label_title2,SIGNAL(clicked(const QString &)),this,SLOT(label_look(const QString &)));//公告栏查看
    connect(ui->label_title3,SIGNAL(clicked(const QString &)),this,SLOT(label_look(const QString &)));//公告栏查看

    connect(ui->pushButton_next_page,SIGNAL(clicked()),this,SLOT(pushButton_next_page()));//下一页
    connect(ui->pushButton_before_page,SIGNAL(clicked()),this,SLOT(pushButton_before_page()));//上一页

    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(combobox_query(const int&)));//查询切换
    connect(ui->pushButton_export,SIGNAL(clicked()),this,SLOT(exportExcel()));//导出按钮
    connect(exportThread,SIGNAL(finish()),this,SLOT(exportExcelFinish()));

    connect(ui->pushButton_search,SIGNAL(clicked()),this,SLOT(pushButton_search()));//搜素按钮

    connect(ui->tabWidget_2,SIGNAL(tabBarClicked(int)),this,SLOT(getTabIndex(const int&)));
}

//返回按键
void Aunt::pushButtonBack()
{
    emit signal_backWindow();
    this->close();
}

//--------------------切换按钮-------------
void Aunt::pushButton_index()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->label_guide->setText("当前位置：首页");
    ui->pushButton_index->setChecked(true);
    ui->pushButton_info->setChecked(false);
    ui->pushButton_data->setChecked(false);
}

void Aunt::pushButton_info()
{
    ui->tabWidget->setCurrentIndex(1);
    ui->label_guide->setText("当前位置：信息管理");
    ui->pushButton_index->setChecked(false);
    ui->pushButton_info->setChecked(true);
    ui->pushButton_data->setChecked(false);
}

void Aunt::pushButton_data()
{
    ui->tabWidget->setCurrentIndex(2);
    refresh_label();
    ui->label_guide->setText("当前位置：公告查看");
    ui->pushButton_index->setChecked(false);
    ui->pushButton_info->setChecked(false);
    ui->pushButton_data->setChecked(true);
}

void Aunt::initRelease()
{
    ui->toolBox->setItemText(0,"");
    ui->textEdit_1->setText("");
    ui->toolBox->setItemText(1,"");
    ui->textEdit_2->setText("");
    ui->toolBox->setItemText(2,"");
    ui->textEdit_3->setText("");
    ui->toolBox->setItemText(3,"");
    ui->textEdit_4->setText("");
    ui->toolBox->setItemText(4,"");
    ui->textEdit_5->setText("");

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

//--------------切换按钮------------------

void Aunt::setLabelName(const QString &name)
{
    ui->label_name->setText(name);
}

void Aunt::setArea(const QString &area)
{
    m_area = area;
}

//----------公告查看---------------------
void Aunt::label_look(const QString &title)
{
    ui->label_release_result_2->clear();
    ui->label_guide->setText("公告修改");
    ui->tabWidget->setCurrentIndex(3);
    ui->lineEdit_look_title->setText(title);
    QSqlQuery query(m_db);
    QString str = "select 公告主题,公告内容 from `announcement` where 公告标题='"+title+"';";
    query.exec(str);
    if(query.next())
    {
        ui->lineEdit_look_subject->setText(query.value(0).toString());
        ui->textEdit_look_content->setText(query.value(1).toString());
    }
}

void Aunt::refresh_label()
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

void Aunt::pushButton_next_page()
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

void Aunt::pushButton_before_page()
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

//----------公告查看---------------------

//-----------信息查询-------------------
void Aunt::queryFunction(const QString &get_row, const QString &str, const QString &tableName, bool flag)
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

void Aunt::queryUnusual()
{
    QString get_row = "SELECT count(*) FROM `information` WHERE 是否正常='异常' AND 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"');";
    QString str="SELECT * FROM `information` WHERE 是否正常='异常' AND 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"') ORDER BY 学号 ASC;";
    queryFunction(get_row,str,"information",true);
}

void Aunt::queryInfo()
{
    QString get_row = "SELECT COUNT(*) FROM `information` where 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"');";
    QString str="SELECT * FROM `information` where 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"') ORDER BY 学号 ASC;";
    queryFunction(get_row,str,"information",true);
}

void Aunt::queryArchive()
{
    QString get_row = "SELECT COUNT(*) FROM `archive` where 宿舍区域='"+m_area+"';";//获取行数
    QString str = "select * from `archive` where 宿舍区域='"+m_area+"' order by 学号 asc;";//全部数据
    queryFunction(get_row,str,"archive");
}

void Aunt::queryDormitory()
{
    QString get_row = "SELECT COUNT(*) FROM `dormitory` where 宿舍区域='"+m_area+"';";
    QString str = "SELECT * FROM `dormitory` where 宿舍区域='"+m_area+"' ORDER BY 楼栋号 ASC, 大寝号 asc, 小寝号 asc, 床号 asc;";
    queryFunction(get_row,str,"dormitory");
}

void Aunt::combobox_query(const int &index)
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
    default:
        break;
    }
}

void Aunt::exportExcel()
{
    QString filepath=QFileDialog::getSaveFileName(this,tr("Save as"),"datafile",tr(" (*.xlsx);;(*.xls)"));
    if(filepath.isEmpty())
        return;

    exportThread->getFilepath(filepath);
    loading->show();

    QString get_row,str;

    switch (ui->comboBox->currentIndex()) {
    case 0:
        get_row = "SELECT count(*) FROM `information` WHERE 是否正常='异常' AND 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"');";
        str="SELECT * FROM `information` WHERE 是否正常='异常' AND 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"') ORDER BY 学号 ASC;";
        exportThread->getInfo(get_row,str,"information",true);
        break;
    case 1:
        get_row = "SELECT COUNT(*) FROM `information` where 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"');";
        str="SELECT * FROM `information` where 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"') ORDER BY 学号 ASC;";
        exportThread->getInfo(get_row,str,"information",true);
        break;
    case 2:
        get_row = "SELECT COUNT(*) FROM `archive` where 宿舍区域='"+m_area+"';";//获取行数
        str = "select * from `archive` where 宿舍区域='"+m_area+"' order by 学号 asc;";//全部数据
        exportThread->getInfo(get_row,str,"archive");
        break;
    case 3:
        get_row = "SELECT COUNT(*) FROM `dormitory` where 宿舍区域='"+m_area+"';";
        str = "SELECT * FROM `dormitory` where 宿舍区域='"+m_area+"' ORDER BY 楼栋号 ASC, 大寝号 asc, 小寝号 asc, 床号 asc;";
        exportThread->getInfo(get_row,str,"dormitory");
        break;
    default:
        return;
    }
    exportThread->start();

}

void Aunt::exportExcelFinish()
{
    loading->close();
    exportThread->exit();
    QMessageBox::information(this,"提示","导出完成",QMessageBox::Ok);
}

void Aunt::pushButton_search()
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
        get_row = "SELECT count(*) FROM `"+tableName+"` WHERE CONCAT("+concat+") LIKE '%"+line+"%' and 是否正常='异常' and 学号 in (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"');";
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
        QString str = "SELECT * FROM `"+tableName+"` WHERE CONCAT("+concat+") LIKE '%"+line+"%' and 学号 in (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"');";
        if(case0)
            str = "SELECT * FROM `"+tableName+"` WHERE CONCAT("+concat+") LIKE '%"+line+"%' and 是否正常='异常' and 学号 in (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"');";
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

void Aunt::keyPressEvent(QKeyEvent *event)
{
    if( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return )
    {
        pushButton_search();
    }
}

void Aunt::getTabIndex(const int& index)
{
    switch (index) {
    case 0:
        queryIndoor();
        break;
    case 1:
        queryOutdoor();
        break;
    default:
        return;
    }
}

void Aunt::queryIndoor()
{
    QString get_row = "SELECT COUNT(*) FROM `indoor` where 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"');";//获取行数
    QString str = "select * FROM `indoor` where 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"') order by 进门时间 desc;";//全部数据
    ui->tableWidget_2->clearContents();
    int row = 0;
    QSqlQuery query(m_db);
    QStringList header;
    QString head = "show columns from `indoor`;";
    query.exec(head);
    int index=0;
    while(query.next())
    {
        header << query.value(0).toString();
        index++;
    }
    ui->tableWidget_2->setColumnCount(index);//设置列数
    ui->tableWidget_2->setHorizontalHeaderLabels(header);//设置标头
    for(int i=0;i<index;++i)
        ui->tableWidget_2->setColumnWidth(i,160);
    query.exec(get_row);
    if(query.first())
    {
        row=query.value("COUNT(*)").toInt();//获取行数
        ui->tableWidget_2->setRowCount(row);//设置行数
    }
    if(row)
    {
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        ui->tableWidget_2->verticalHeader()->setVisible(false);//隐藏列头
        ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);//只允许单选
        ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget_2->setAlternatingRowColors(true);
        query.exec(str);
        for(int i = 0; query.next(); i++)
        {
            for (int j = 0; j < index; j++)
            {
                if(j==2)
                {
                    QDateTime time;
                    time = query.value(j).toDateTime();
                    QString strBuffer;
                    strBuffer = time.toString("yyyy-MM-dd hh:mm:ss");
                    ui->tableWidget_2->setItem(i,j,new QTableWidgetItem(strBuffer));
                    ui->tableWidget_2->item(i,j)->setTextAlignment(Qt::AlignCenter);
                }
                else
                {
                    ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(query.value(j).toString()));
                    ui->tableWidget_2->item(i,j)->setTextAlignment(Qt::AlignCenter);
                }
            }
        }
    }
}

void Aunt::queryOutdoor()
{
    QString get_row = "SELECT COUNT(*) FROM `outdoor` where 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"');";//获取行数
    QString str = "select * FROM `outdoor` where 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"')  order by 出门时间 desc;";//全部数据
    ui->tableWidget_3->clearContents();
    int row = 0;
    QSqlQuery query(m_db);
    QStringList header;
    QString head = "show columns from `outdoor`;";
    query.exec(head);
    int index=0;
    while(query.next())
    {
        header << query.value(0).toString();
        index++;
    }
    ui->tableWidget_3->setColumnCount(index);//设置列数
    ui->tableWidget_3->setHorizontalHeaderLabels(header);//设置标头
    for(int i=0;i<index;++i)
        ui->tableWidget_3->setColumnWidth(i,160);
    query.exec(get_row);
    if(query.first())
    {
        row=query.value("COUNT(*)").toInt();//获取行数
        ui->tableWidget_3->setRowCount(row);//设置行数
    }
    if(row)
    {
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        ui->tableWidget_3->verticalHeader()->setVisible(false);//隐藏列头
        ui->tableWidget_3->setSelectionMode(QAbstractItemView::SingleSelection);//只允许单选
        ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget_3->setAlternatingRowColors(true);
        query.exec(str);
        for(int i = 0; query.next(); i++)
        {
            for (int j = 0; j < index; j++)
            {
                if(j==2)
                {
                    QDateTime time;
                    time = query.value(j).toDateTime();
                    QString strBuffer;
                    strBuffer = time.toString("yyyy-MM-dd hh:mm:ss");
                    ui->tableWidget_3->setItem(i,j,new QTableWidgetItem(strBuffer));
                    ui->tableWidget_3->item(i,j)->setTextAlignment(Qt::AlignCenter);
                }
                else
                {
                    ui->tableWidget_3->setItem(i, j, new QTableWidgetItem(query.value(j).toString()));
                    ui->tableWidget_3->item(i,j)->setTextAlignment(Qt::AlignCenter);
                }
            }
        }
    }
}
//-----------信息查询-------------------
