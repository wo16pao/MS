#include "admin_modifyarchive.h"
#include "ui_admin_modifyarchive.h"
#include <QtSql/QSqlQuery>

Admin_ModifyArchive::Admin_ModifyArchive(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Admin_ModifyArchive)
{
    initTitleBar();
    ui->setupUi(this);
    db = QSqlDatabase::database("mysql_connect");
    initConnection();
    initCombobox();

}

Admin_ModifyArchive::~Admin_ModifyArchive()
{
    delete ui;
}

void Admin_ModifyArchive::modifyArchive()
{
    QString id =        ui->lineEdit_id->text();
    QString name =      ui->lineEdit_name->text();
    QString sex =       ui->comboBox->currentText();
    QString phone =     ui->lineEdit_phone->text();
    QString birth =     ui->dateEdit->date().toString("yyyy-MM-dd");
    QString adress =    ui->lineEdit_address->text();
    QString entrance =  ui->dateEdit_2->date().toString("yyyy-MM-dd");
    QString dean =      ui->comboBox_dean->currentText();
    QString dean2 =     ui->comboBox_dean2->currentText();
    QString dean3 =     ui->comboBox_dean3->currentText();
    QString dorm =      ui->comboBox_dorm->currentText();
    QString dorm2 =     ui->comboBox_dorm2->currentText();
    QString dorm3 =     ui->comboBox_dorm3->currentText();
    QString dorm4 =     ui->comboBox_dorm4->currentText();
    QString dorm5 =     ui->comboBox_dorm5->currentText();
    QString dean4 =     ui->comboBox_dean4->currentText();
    QString num =       ui->lineEdit_num->text();

    if(id.isEmpty()||name.isEmpty()||phone.isEmpty()||adress.isEmpty()||num.isEmpty())
    {
        ui->label_result->setText("信息不能为空");
        return;
    }

    QString str = "update `archive` set 学号='"+id+"',姓名='"+name+"',性别='"+sex+"',联系电话='"+phone+"',出生年月='"+birth+"',家庭住址='"+adress+"',入学时间='"+entrance+"',学院名称='"+dean+"',专业名称='"+dean2+"',班级号='"+dean3+"',宿舍区域='"+dorm+"',楼栋号='"+dorm2+"',大寝号='"+dorm3+"',小寝号='"+dorm4+"',床号='"+dorm5+"',辅导员='"+dean4+"',出入次数='"+num+"' where 学号='"+m_id+"';";
    QSqlDatabase db;
    db = QSqlDatabase::database("mysql_connect");
    QSqlQuery query(db);
    if(query.exec(str))
    {
        ui->label_result->setText("修改成功");
        emit refresh();
    }
    else {
        ui->label_result->setText("修改失败");
    }
}

void Admin_ModifyArchive::pushButton_back()
{
    ui->label_result->clear();
    ui->lineEdit_id->clear();
    ui->lineEdit_num->clear();
    ui->lineEdit_name->clear();
    ui->lineEdit_phone->clear();
    ui->lineEdit_address->clear();
    ui->comboBox_dean->clear();
    ui->comboBox_dean2->clear();
    ui->comboBox_dean3->clear();
    ui->comboBox_dean4->clear();
    ui->comboBox_dorm->clear();
    ui->comboBox_dorm2->clear();
    ui->comboBox_dorm3->clear();
    ui->comboBox_dorm4->clear();
    ui->comboBox_dorm5->clear();
    this->close();
}


//-----------------------同addArchive------------//
void Admin_ModifyArchive::initConnection()
{
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(modifyArchive()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(pushButton_back()));

    connect(ui->comboBox_dean,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDean2(QString)));
    connect(ui->comboBox_dean2,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDean3(QString)));
    connect(ui->comboBox_dean3,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDean4(QString)));

    connect(ui->comboBox_dorm,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDorm2(QString)));
    connect(ui->comboBox_dorm2,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDorm3(QString)));
    connect(ui->comboBox_dorm3,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDorm4(QString)));
    connect(ui->comboBox_dorm4,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDorm5(QString)));
}

void Admin_ModifyArchive::getArchive(QString text)
{
    initArchive(text);
    m_id = text;
}

void Admin_ModifyArchive::initArchive(QString text)
{
    QSqlQuery query(db);
    QString str = "select * from `archive` where 学号='"+text+"';";
    query.exec(str);
    int index=0;
    if(query.next())
    {
        ui->lineEdit_id->setText(query.value(index++).toString());
        ui->lineEdit_name->setText(query.value(index++).toString());
        ui->comboBox->setCurrentText(query.value(index++).toString());
        ui->lineEdit_phone->setText(query.value(index++).toString());
        ui->dateEdit->setDate(query.value(index++).toDate());
        ui->lineEdit_address->setText(query.value(index++).toString());
        ui->dateEdit_2->setDate(query.value(index++).toDate());
        ui->comboBox_dean->setCurrentText(query.value(index++).toString());
        ui->comboBox_dean2->setCurrentText(query.value(index++).toString());
        ui->comboBox_dean3->setCurrentText(query.value(index++).toString());
        ui->comboBox_dorm->setCurrentText(query.value(index++).toString());
        ui->comboBox_dorm2->setCurrentText(query.value(index++).toString());
        ui->comboBox_dorm3->setCurrentText(query.value(index++).toString());
        ui->comboBox_dorm4->setCurrentText(query.value(index++).toString());
        ui->comboBox_dorm5->setCurrentText(query.value(index++).toString());
        ui->comboBox_dean4->setCurrentText(query.value(index++).toString());
        ui->lineEdit_num->setText(query.value(index++).toString());
    }
}

void Admin_ModifyArchive::initDean()
{
    QSqlQuery query(db);
    QStringList header;
    QString str = "SELECT DISTINCT(学院名称) FROM dean ORDER BY 学院名称";
    query.exec(str);
    while(query.next())
    {
        header << query.value(0).toString();
    }
    ui->comboBox_dean->addItems(header);
}

void Admin_ModifyArchive::initDean2(QString text)
{
    ui->comboBox_dean2->clear();
    QSqlQuery query(db);
    QStringList header;
    QString str="select distinct(专业名称) from dean where 学院名称 = '"+text+"' ORDER BY 专业名称;";
    query.exec(str);
    while(query.next())
    {
        header << query.value(0).toString();
    }
    ui->comboBox_dean2->addItems(header);
}

void Admin_ModifyArchive::initDean3(QString text)
{
    ui->comboBox_dean3->clear();
    QSqlQuery query(db);
    QStringList header;
    QString str="select distinct(班级号) from dean where 专业名称 = '"+text+"' ORDER BY 班级号;";
    query.exec(str);
    while(query.next())
    {
        header << query.value(0).toString();
    }
    ui->comboBox_dean3->addItems(header);
}

void Admin_ModifyArchive::initDean4(QString text)
{
    ui->comboBox_dean4->clear();
    QSqlQuery query(db);
    QStringList header;
    QString str="select distinct(辅导员) from dean where 班级号 = '"+text+"' ORDER BY 辅导员;";
    query.exec(str);
    while(query.next())
    {
        header << query.value(0).toString();
    }
    ui->comboBox_dean4->addItems(header);
}

void Admin_ModifyArchive::initDorm()
{
    ui->comboBox_dorm->clear();
    QSqlQuery query(db);
    QStringList header;
    QString str = "SELECT DISTINCT(宿舍区域) FROM `dormitory` ORDER BY 宿舍区域";
    query.exec(str);
    while(query.next())
    {
        header << query.value(0).toString();
    }
    ui->comboBox_dorm->addItems(header);
}

void Admin_ModifyArchive::initDorm2(QString text)
{
    ui->comboBox_dorm2->clear();
    QSqlQuery query(db);
    QStringList header;
    QString str = "SELECT DISTINCT(楼栋号) FROM `dormitory` where 宿舍区域='"+text+"' ORDER BY 楼栋号";
    query.exec(str);
    while(query.next())
    {
        header << query.value(0).toString();
    }
    ui->comboBox_dorm2->addItems(header);
}

void Admin_ModifyArchive::initDorm3(QString text)
{
    ui->comboBox_dorm3->clear();
    QSqlQuery query(db);
    QStringList header;
    QString str = "SELECT DISTINCT(大寝号) FROM `dormitory` where 宿舍区域='"+ui->comboBox_dorm->currentText()+"' and 楼栋号='"+text+"' ORDER BY 大寝号";
    query.exec(str);
    while(query.next())
    {
        header << query.value(0).toString();
    }
    ui->comboBox_dorm3->addItems(header);
}

void Admin_ModifyArchive::initDorm4(QString text)
{
    ui->comboBox_dorm4->clear();
    QSqlQuery query(db);
    QStringList header;
    QString str = "SELECT DISTINCT(小寝号) FROM `dormitory` where 宿舍区域='"+ui->comboBox_dorm->currentText()+"' and 楼栋号='"+ui->comboBox_dorm2->currentText()+"' and 大寝号='"+text+"' ORDER BY 小寝号";
    query.exec(str);
    while(query.next())
    {
        header << query.value(0).toString();
    }
    ui->comboBox_dorm4->addItems(header);
}

void Admin_ModifyArchive::initDorm5(QString text)
{
    ui->comboBox_dorm5->clear();
    QSqlQuery query(db);
    QStringList header;
    QString str = "SELECT DISTINCT(床号) FROM `dormitory` where 宿舍区域='"+ui->comboBox_dorm->currentText()+"' and 楼栋号='"+ui->comboBox_dorm2->currentText()+"' and 大寝号='"+ui->comboBox_dorm3->currentText()+"' and 小寝号='"+text+"' ORDER BY 床号";
    query.exec(str);
    while(query.next())
    {
        header << query.value(0).toString();
    }
    ui->comboBox_dorm5->addItems(header);
}

void Admin_ModifyArchive::initCombobox()
{
    initDean();
    initDean2(ui->comboBox_dean->currentText());
    initDean3(ui->comboBox_dean2->currentText());
    initDean4(ui->comboBox_dean3->currentText());

    initDorm();
    initDorm2(ui->comboBox_dorm->currentText());
    initDorm3(ui->comboBox_dorm2->currentText());
    initDorm4(ui->comboBox_dorm3->currentText());
    initDorm5(ui->comboBox_dorm4->currentText());
}

void Admin_ModifyArchive::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(56,70,85);
    m_titleBar->setTitleIcon(":/icon.png");
    m_titleBar->setTitleContent(QStringLiteral("我的窗口"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
