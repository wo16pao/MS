#include "admin_addarchive.h"
#include "ui_admin_addarchive.h"
#include <QtSql/QSqlQuery>

Admin_AddArchive::Admin_AddArchive(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_AddArchive)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("mysql_connect");
    initConnection();
    initCombobox();

}

Admin_AddArchive::~Admin_AddArchive()
{
    delete ui;
}

void Admin_AddArchive::initConnection()
{
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(addArchive()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(pushButton_back()));

    connect(ui->comboBox_dean,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDean2(QString)));
    connect(ui->comboBox_dean2,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDean3(QString)));
    connect(ui->comboBox_dean3,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDean4(QString)));

    connect(ui->comboBox_dorm,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDorm2(QString)));
    connect(ui->comboBox_dorm2,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDorm3(QString)));
    connect(ui->comboBox_dorm3,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDorm4(QString)));
    connect(ui->comboBox_dorm4,SIGNAL(currentIndexChanged(QString)),this,SLOT(initDorm5(QString)));
}

void Admin_AddArchive::addArchive()
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

    QString str = "insert into `archive` values ('"+id+"','"+name+"','"+sex+"','"+phone+"','"+birth+"','"+adress+"','"+entrance+"','"+dean+"','"+dean2+"','"+dean3+"','"+dorm+"','"+dorm2+"','"+dorm3+"','"+dorm4+"','"+dorm5+"','"+dean4+"','"+num+"');";
    QSqlDatabase db;
    db = QSqlDatabase::database("mysql_connect");
    QSqlQuery query(db);
    if(query.exec(str))
    {
        ui->label_result->setText("添加成功");
        emit refresh();
    }
    else {
        ui->label_result->setText("添加失败，请检查信息是否填写正确");
    }

}

void Admin_AddArchive::pushButton_back()
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

void Admin_AddArchive::initDean()
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

void Admin_AddArchive::initDean2(QString text)
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

void Admin_AddArchive::initDean3(QString text)
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

void Admin_AddArchive::initDean4(QString text)
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

void Admin_AddArchive::initDorm()
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

void Admin_AddArchive::initDorm2(QString text)
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

void Admin_AddArchive::initDorm3(QString text)
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

void Admin_AddArchive::initDorm4(QString text)
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

void Admin_AddArchive::initDorm5(QString text)
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

void Admin_AddArchive::initCombobox()
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
