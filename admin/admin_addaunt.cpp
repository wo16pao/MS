#include "admin_addaunt.h"
#include "ui_admin_addaunt.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

Admin_AddAunt::Admin_AddAunt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_AddAunt)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(addAunt()));
        connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(pushButton_back()));

}

Admin_AddAunt::~Admin_AddAunt()
{
    delete ui;
}

void Admin_AddAunt::addAunt()
{
    QString name = ui->lineEdit->text();
    QString id = ui->lineEdit_2->text();
    QString password = ui->lineEdit_3->text();
    QString area = ui->comboBox->currentText();

    if(name.isEmpty()||id.isEmpty()||password.isEmpty())
    {
        ui->label_result->setText("信息不能为空");
        return;
    }

    QString str = "insert into `aunt` values ('"+name+"','"+id+"','"+password+"','"+area+"');";
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

void Admin_AddAunt::pushButton_back()
{
    ui->label_result->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    this->close();
}
