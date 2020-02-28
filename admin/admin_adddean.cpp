#include "admin_adddean.h"
#include "ui_admin_adddean.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

Admin_AddDean::Admin_AddDean(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_AddDean)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(addDean()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(pushButton_back()));
}

Admin_AddDean::~Admin_AddDean()
{
    delete ui;
}

void Admin_AddDean::addDean()
{
    QString name = ui->lineEdit->text();
    QString major = ui->lineEdit_2->text();
    QString classId = ui->lineEdit_3->text();
    QString teacher = ui->lineEdit_4->text();
    if(name.isEmpty()||major.isEmpty()||classId.isEmpty()||teacher.isEmpty())
    {
        ui->label_result->setText("信息不能为空");
        return;
    }

    QString str = "insert into `dean` values ('"+name+"','"+major+"','"+classId+"','"+teacher+"');";
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

void Admin_AddDean::pushButton_back()
{
    ui->label_result->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    this->close();
}
