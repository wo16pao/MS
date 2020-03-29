#include "widget.h"
#include "ui_widget.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QString>
#include <QAction>
#include <QSettings>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Widget)
{
    initTitleBar();

    ui->setupUi(this);
    startTimer(1000);
    SqlConnect();//首先连接数据库

    connect(ui->pushButton_login,SIGNAL(clicked()),this,SLOT(Login()));

    //动画部分初始化
    pPosAnimation1 = new QPropertyAnimation(ui->label_du1, "pos");
    pPosAnimation2 = new QPropertyAnimation(ui->label_du2, "pos");
    pPosGroup = new QSequentialAnimationGroup(this);
    pPosGroup2 = new QSequentialAnimationGroup(this);
    group2 = new QParallelAnimationGroup(this);
    group = new QParallelAnimationGroup(this);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    speed1x = 5+qrand()%10;
    speed1y = 5+qrand()%10;
    speed2x = 5+qrand()%10;
    speed2y = 5+qrand()%10;

    QString isChecked;
    QString account;
    QString password;
    readInit("isChecked",isChecked);
    if(isChecked=="0"){
        readInit("account",account);
        ui->lineEdit_account->setText(account);
    }
    else if(isChecked=="1"){
        readInit("aunt_account",account);
        readInit("aunt_password",password);
        ui->lineEdit_account->setText(account);
        ui->lineEdit_password->setText(password);
        ui->checkBox->setChecked(true);
        ui->radioButton_aunt->setChecked(true);
    }
    else if(isChecked=="2"){
        readInit("admin_account",account);
        readInit("admin_password",password);
        ui->lineEdit_account->setText(account);
        ui->lineEdit_password->setText(password);
        ui->checkBox->setChecked(true);
        ui->radioButton_admin->setChecked(true);
    }
}

Widget::~Widget()
{
    if(!ui->checkBox->isChecked()){
        writeInit("isChecked","0");
        writeInit("account",ui->lineEdit_account->text());
    }
    delete m_aunt;
    delete m_adm;
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
        return;
    }

    QSqlQuery query(db);
    if(ui->radioButton_aunt->isChecked())
    {
        //宿管登录
        string stdPsw = userPsw.toStdString();
        stdPsw = MD5(stdPsw).toStr();
        QString MD5Psw = QString::fromStdString(stdPsw);
        query.exec("select count(账号) from `aunt` where 账号 = '"+userId+"' and 密码 = '"+MD5Psw+"';");
    }
    else if(ui->radioButton_admin->isChecked())
    {
        //管理员登陆
        string stdPsw = userPsw.toStdString();
        stdPsw = MD5(stdPsw).toStr();
        QString MD5Psw = QString::fromStdString(stdPsw);
        query.exec("select count(账号) from manager where 账号 = '"+userId+"' and 密码 = '"+MD5Psw+"';");
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
            if(ui->checkBox->isChecked()){
                writeInit("aunt_account",userId);
                writeInit("aunt_password",userPsw);
                writeInit("isChecked","1");
            }


            m_aunt = new Aunt;
            QString temp = "宿管：";
            QString get_name = "select 姓名,宿舍区域 from `aunt` where 账号= '"+userId+"' and 密码 = '"+userPsw+"';";
            query.exec(get_name);
            if(query.next())
            {
                m_aunt->setLabelName(temp+query.value(0).toString());
                m_aunt->setArea(query.value(1).toString());
                m_aunt->afterInit();
            }
            m_aunt->show();
            connect(m_aunt,SIGNAL(signal_backWindow()),this,SLOT(Reshow()));//接受重新打开窗口信号
        }
        else if(ui->radioButton_admin->isChecked())
        {
            //管理员登陆
            if(ui->checkBox->isChecked()){
                writeInit("admin_account",userId);
                writeInit("admin_password",userPsw);
                writeInit("isChecked","2");
            }

            string stdPsw = userPsw.toStdString();
            stdPsw = MD5(stdPsw).toStr();
            userPsw = QString::fromStdString(stdPsw);
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
    this->show();
}

//清理界面
void Widget::ClearUI()
{
    ui->lineEdit_password->clear();
}

void Widget::timerEvent(QTimerEvent *e)
{
    animation();
}

void Widget::writeInit(const QString &key, const QString &value)
{
    QString path = "account.ini";

    //创建配置文件操作对象
    QSettings *config = new QSettings(path, QSettings::IniFormat);

    //将信息写入配置文件
    config->beginGroup("config");
    config->setValue(key, value);
    config->endGroup();
    delete config;
}

void Widget::readInit(const QString &key, QString &value)
{
    value = QString("");
    QString path = "account.ini";

    //创建配置文件操作对象
    QSettings *config = new QSettings(path, QSettings::IniFormat);

    //读取配置信息
    value = config->value(QString("config/") + key).toString();
    delete config;

}

void Widget::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(62,96,147);
    m_titleBar->setTitleIcon(":/icon.png");
    m_titleBar->setTitleContent(QStringLiteral("我的窗口"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}

void Widget::animation()
{
    static int du1x = 40, du1y = 50;
    static int du2x = 380, du2y = 70;
    //static int speed1x=10,speed1y=10, speed2x=10,speed2y=10;


    pPosAnimation1->setDuration(1000);
    pPosAnimation1->setStartValue(QPoint(du1x, du1y));
    du1x += speed1x;
    if(du1x <= 0){
        du1x = 0;
        speed1x = -speed1x;
    }
    else if(du1x >= this->width()-ui->label_du1->width()){
        du1x = this->width()-ui->label_du1->width();
        speed1x = -speed1x;
    }
    du1y += speed1y;
    if(du1y <= 0){
        du1y = 0;
        speed1y = -speed1y;
    }
    else if(du1y >= this->height()-ui->label_du1->height()){
        du1y = this->height()-ui->label_du1->height();
        speed1y = -speed1y;
    }
    pPosAnimation1->setEndValue(QPoint(du1x, du1y));
    pPosAnimation1->setEasingCurve(QEasingCurve::Linear);


    pPosAnimation2->setDuration(1000);
    pPosAnimation2->setStartValue(QPoint(du2x, du2y));
    du2x += speed2x;
    if(du2x <= 0){
        du2x = 0;
        speed2x = -speed2x;
    }
    else if(du2x >= this->width()-ui->label_du2->width()){
        du2x = this->width()-ui->label_du2->width();
        speed2x = -speed2x;
    }
    du2y += speed2y;
    if(du2y <= 0){
        du2y = 0;
        speed2y = -speed2y;
    }
    else if(du2y >= this->height()-ui->label_du2->height()){
        du2y = this->height()-ui->label_du2->height();
        speed2y = -speed2y;
    }
    pPosAnimation2->setEndValue(QPoint(du2x, du2y));
    pPosAnimation2->setEasingCurve(QEasingCurve::Linear);

    pPosGroup->addAnimation(pPosAnimation1);
    pPosGroup2->addAnimation(pPosAnimation2);

    group->addAnimation(pPosGroup);
    group->setLoopCount(1);
    group->start();
    group2->addAnimation(pPosGroup2);
    group2->setLoopCount(1);
    group2->start();
}

void Widget::rememberPassword()
{

}

