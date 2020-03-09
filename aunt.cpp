#include "aunt.h"
#include "ui_aunt.h"
#include <QDateTime>

Aunt::Aunt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Aunt)
{
    ui->setupUi(this);
    init();
    initConnection();

}

Aunt::~Aunt()
{
    delete ui;
}

//初始化
void Aunt::init()
{
    m_db = QSqlDatabase::database("mysql_connect");
    ui->tabWidget->tabBar()->hide();//隐藏页头
    initRelease();

    m_page = 0;//初始化公告当前页数
    m_page_flag=false;
    sort_lable(0);
}
//初始化连接
void Aunt::initConnection()
{
    connect(ui->pushButton_back,SIGNAL(clicked()),this,SLOT(pushButtonBack()));//返回按钮
    //左侧按钮-------------------------------------------------
    ui->tabWidget->tabBar()->hide();//隐藏tab
    connect(ui->pushButton_index,SIGNAL(clicked()),this,SLOT(pushButtonIndex()));
    connect(ui->pushButton_info,SIGNAL(clicked()),this,SLOT(pushButtonInfo()));
    connect(ui->pushButton_release,SIGNAL(clicked()),this,SLOT(pushButtonBulletin()));
    //左侧按钮-------------------------------------------------

    connect(ui->label_title1,SIGNAL(clicked(const QString &)),this,SLOT(lable_look(const QString &)));//公告栏查看
    connect(ui->label_title2,SIGNAL(clicked(const QString &)),this,SLOT(lable_look(const QString &)));//公告栏查看
    connect(ui->label_title3,SIGNAL(clicked(const QString &)),this,SLOT(lable_look(const QString &)));//公告栏查看
    connect(ui->label_title4,SIGNAL(clicked(const QString &)),this,SLOT(lable_look(const QString &)));//公告栏查看
    connect(ui->label_title5,SIGNAL(clicked(const QString &)),this,SLOT(lable_look(const QString &)));//公告栏查看

    connect(ui->pushButton_next_page,SIGNAL(clicked()),this,SLOT(pushButton_next_page()));//下一页
    connect(ui->pushButton_before_page,SIGNAL(clicked()),this,SLOT(pushButton_before_page()));//上一页

        connect(ui->pushButton_modify_back,SIGNAL(clicked()),this,SLOT(pushButtonBulletin()));//查看公告返回键

        connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(combobox_query(const int&)));//查询切换
}

//返回按键
void Aunt::pushButtonBack()
{
    emit signal_backWindow();
    this->close();
}

//---------------------按钮切换页面---------------------------------------------//
//主页
void Aunt::pushButtonIndex()
{
    ui->tabWidget->setCurrentIndex(0);
}
//数据管理
void Aunt::pushButtonInfo()
{
    ui->tabWidget->setCurrentIndex(1);
}

void Aunt::pushButtonBulletin()
{
    ui->tabWidget->setCurrentIndex(2);
    sort_lable(0);
     ui->label_guide->setText("当前位置：公告查看");
}
//-----------------------------------------------------------------------------------//
//-----------------------------公告信息--------------------------------
void Aunt::lable_look(const QString &title)
{
    ui->tabWidget->setCurrentIndex(3);
    ui->lineEdit_look_title->setText(title);
    m_bulletin_title = title;
    QSqlQuery query(m_db);
    QString str = "select 公告内容 from `announcement` where 公告标题='"+title+"';";
    query.exec(str);
    if(query.next())
    {
        ui->textEdit_look_content->setText(query.value(0).toString());
        m_bulletin_content = query.value(0).toString();
    }
}

void Aunt::sort_lable(const int &addOrSub)
{
    m_page+=addOrSub;

    if(addOrSub!=0)
        m_page_flag = false;

    if(m_page<0)//判断是否为第一页
    {
        m_page = 0;
        return;
    }
    if(m_page_flag)//判断是否已经是最后一页
    {
        m_page_flag=false;
        --m_page;
        QString page = "当前页数：" + QString::number(m_page+1);
        ui->label_page->setText(page);
        return;
    }

    QString page = "当前页数：" + QString::number(m_page+1);
    ui->label_page->setText(page);

    int num=m_page*5;
    QString str = "select 公告标题 ,公告内容 from `announcement` order by 发布时间 desc;";
    QSqlQuery query(m_db);
    query.exec(str);
    for(int i=0;i<num;++i)//定位到我所需要的地方
        query.next();

    if(query.next())
    {
        ui->label_title1->setText(query.value(0).toString());
        ui->label_content1->setText(query.value(1).toString());
    }
    else {
        m_page_flag = true;
        this->sort_lable(0);//如果第一项就没有，则回到上一页
        return;
    }

    if(query.next())
    {
        ui->label_title2->setVisible(true);
        ui->label_content2->setVisible(true);
        ui->label_title3->setVisible(true);
        ui->label_content3->setVisible(true);
        ui->label_title4->setVisible(true);
        ui->label_content4->setVisible(true);
        ui->label_title5->setVisible(true);
        ui->label_content5->setVisible(true);
        ui->label_title2->setText(query.value(0).toString());
        ui->label_content2->setText(query.value(1).toString());
    }
    else {
        ui->label_title2->setVisible(false);
        ui->label_content2->setVisible(false);
        ui->label_title3->setVisible(false);
        ui->label_content3->setVisible(false);
        ui->label_title4->setVisible(false);
        ui->label_content4->setVisible(false);
        ui->label_title5->setVisible(false);
        ui->label_content5->setVisible(false);
        m_page_flag=true;
        return;
    }

    if(query.next())
    {

        ui->label_title3->setText(query.value(0).toString());
        ui->label_content3->setText(query.value(1).toString());
    }
    else {
        ui->label_title3->setVisible(false);
        ui->label_content3->setVisible(false);
        ui->label_title4->setVisible(false);
        ui->label_content4->setVisible(false);
        ui->label_title5->setVisible(false);
        ui->label_content5->setVisible(false);
        m_page_flag=true;
        return;
    }

    if(query.next())
    {
        ui->label_title4->setText(query.value(0).toString());
        ui->label_content4->setText(query.value(1).toString());
    }
    else {
        ui->label_title4->setVisible(false);
        ui->label_content4->setVisible(false);
        ui->label_title5->setVisible(false);
        ui->label_content5->setVisible(false);
        m_page_flag=true;
        return;
    }

    if(query.next())
    {
        ui->label_title5->setText(query.value(0).toString());
        ui->label_content5->setText(query.value(1).toString());
    }
    else {
        ui->label_title5->setVisible(false);
        ui->label_content5->setVisible(false);
        m_page_flag=true;
        return;
    }

}

void Aunt::pushButton_next_page()
{
    sort_lable(1);
}

void Aunt::pushButton_before_page()
{
    sort_lable(-1);
}

void Aunt::setLabelName(const QString &name)
{
    ui->label_name->setText(name);
}

void Aunt::initRelease()
{
    ui->lineEdit_1->clear();
    ui->textEdit_1->clear();
    ui->lineEdit_2->clear();
    ui->textEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->textEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->textEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->textEdit_5->clear();

    QSqlQuery query(m_db);
    QString str = "select 公告标题,公告内容 from `announcement` order by 发布时间 desc;";
    query.exec(str);
    if(query.next())
    {
        ui->lineEdit_1->setText(query.value(0).toString());
        ui->textEdit_1->setPlainText(query.value(1).toString());
    }
    if(query.next())
    {
        ui->lineEdit_2->setText(query.value(0).toString());
        ui->textEdit_2->setPlainText(query.value(1).toString());
    }
    if(query.next())
    {
        ui->lineEdit_3->setText(query.value(0).toString());
        ui->textEdit_3->setPlainText(query.value(1).toString());
    }
    if(query.next())
    {
        ui->lineEdit_4->setText(query.value(0).toString());
        ui->textEdit_4->setPlainText(query.value(1).toString());
    }
    if(query.next())
    {
        ui->lineEdit_5->setText(query.value(0).toString());
        ui->textEdit_5->setPlainText(query.value(1).toString());
    }

}
//-----------------------------公告信息--------------------------------
//-------------查询公告------------------------

void Aunt::queryInfo()
{
    QString get_row = "SELECT count(*) FROM `information` WHERE 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"');";
    QString str = "SELECT * FROM `information` WHERE 学号 IN (SELECT 学号 FROM `archive` WHERE 宿舍区域='"+m_area+"') ORDER BY 学号 ASC;";
    queryFunction(get_row,str,"information",true);
}

void Aunt::queryArchive()
{
    QString get_row = "SELECT COUNT(*) FROM `archive` where 宿舍区域='"+m_area+"';";
    QString str = "select * from `archive` where 宿舍区域='"+m_area+"' order by 学号 asc;";//全部数据
    queryFunction(get_row,str,"archive");
}

void Aunt::queryDormitory()
{
    QString get_row = "SELECT COUNT(*) FROM `dormitory` where 宿舍区域='"+m_area+"';";
    QString str = "SELECT * FROM `dormitory` where 宿舍区域='"+m_area+"' ORDER BY 楼栋号 ASC, 大寝号 asc, 小寝号 asc, 床号 asc;";
    queryFunction(get_row,str,"dormitory");
}

void Aunt::combobox_query(const int & index)
{
    switch (index) {
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

void Aunt::setArea(const QString &area)
{
    m_area = area;
    ui->label_area->setText(area);
}
