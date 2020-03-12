#include "widget.h"
#include "ui_widget.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QString>

Widget::Widget(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Widget)
{
//    QFile file(":/qss/addAndModify.qss");
//    if (file.open(QFile::ReadOnly)) {
//            QString qss = QLatin1String(file.readAll());
//            QString paletteColor = qss.mid(20, 7);
//            qApp->setPalette(QPalette(QColor(paletteColor)));
//            qApp->setStyleSheet(qss);
//            file.close();
//        }

    initTitleBar();
    ui->setupUi(this);

    SqlConnect();//首先连接数据库

    connect(ui->pushButton_login,SIGNAL(clicked()),this,SLOT(Login()));
    connect(ui->pushButton_quit,SIGNAL(clicked()),this,SLOT(close()));
}

Widget::~Widget()
{
    delete ui;
}

//连接数据库
void Widget::SqlConnect()
{

    if(QSqlDatabase::contains("mysql_connect"))//若已连接则不重复连接
    {
        db=QSqlDatabase::database("mysql_connect");
    }
    db=QSqlDatabase::addDatabase("QMYSQL","mysql_connect");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("test");
    db.setUserName("root");
    db.setPassword("root");
    while(!db.open())
    {
        if(QMessageBox::Reset==QMessageBox::critical
                (this,"错误","数据库连接失败",QMessageBox::Reset|QMessageBox::Cancel,QMessageBox::Reset))
            continue;
        else
            exit(0);
    }
}

void Widget::Login()
{
    QString userId = ui->lineEdit_account->text();//获取账号
    QString userPsw = ui->lineEdit_password->text();//获取密码
    if(userId.isEmpty()||userPsw.isEmpty())
    {
        QMessageBox::information(this,"提示","请输入信息",QMessageBox::Ok);
    }
    QSqlQuery query(db);
    if(ui->radioButton_aunt->isChecked())
    {
        //宿管登录
        query.exec("select count(账号) from `aunt` where 账号 = '"+userId+"' and 密码 = '"+userPsw+"';");
    }
//    else if(ui->radioButton_teacher->isChecked())
//    {
//        //辅导员登录
//    }
    else if(ui->radioButton_admin->isChecked())
    {
        //管理员登陆
        query.exec("select count(账号) from manager where 账号 = '"+userId+"' and 密码 = '"+userPsw+"';");
    }
    bool flag=false;//判断是否登录成功
    while(query.next())//判断是否存在，若存在则应为1
    {
        if(query.value(0).toInt())
        {
            flag = true;
            break;
        }
    }
    if(flag)//登录成功
    {
        this->hide();
        if(ui->radioButton_aunt->isChecked())
        {
            //宿管登录
            m_aunt = new Aunt;
            QString temp = "管理员：";
            QString get_name = "select 姓名,宿舍区域 from `aunt` where 账号= '"+userId+"' and 密码 = '"+userPsw+"';";
            query.exec(get_name);
            if(query.next())
            {
                m_aunt->setLabelName(temp+query.value(0).toString());
                m_aunt->setArea(query.value(1).toString());
            }
            m_aunt->show();
            connect(m_aunt,SIGNAL(signal_backWindow()),this,SLOT(Reshow()));//接受重新打开窗口信号
        }
        else if(ui->radioButton_admin->isChecked())
        {
            //管理员登陆

            m_adm = new Admin;
            QString temp = "管理员：";
            QString get_name = "select 姓名 from `manager` where 账号= '"+userId+"' and 密码 = '"+userPsw+"';";
            query.exec(get_name);
            if(query.next())
                m_adm->setLabelName(temp+query.value(0).toString());
            m_adm->show();
            connect(m_adm,SIGNAL(signal_backWindow()),this,SLOT(Reshow()));//接受重新打开窗口信号

        }
    }
    else //登录失败
    {
        QMessageBox::information(this,"提示","账号或密码错误",QMessageBox::Ok);
        ClearUI();
    }
}

void Widget::Reshow()
{
    ClearUI();
    this->show();
}

//清理界面
void Widget::ClearUI()
{
    ui->lineEdit_password->clear();
}

void Widget::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(56,70,85);
    m_titleBar->setTitleIcon(":/icon.png");
    m_titleBar->setTitleContent(QStringLiteral("我的窗口"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
