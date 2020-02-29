#include "admin_addinfo.h"
#include "ui_admin_addinfo.h"
#include <QtSql/QSqlQuery>

Admin_AddInfo::Admin_AddInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_AddInfo)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("mysql_connect");

    connect(ui->pushButton_confirm,SIGNAL(clicked()),this,SLOT(addInfo()));
    connect(ui->pushButton_quit,SIGNAL(clicked()),this,SLOT(pushButton_back()));
    connect(ui->lineEdit_id,SIGNAL(editingFinished()),this,SLOT(checkId()));
}

Admin_AddInfo::~Admin_AddInfo()
{
    delete ui;
}

void Admin_AddInfo::addInfo()
{
    QString id = ui->lineEdit_id->text();
    QString name = ui->lineEdit_name->text();
    QString out = ui->timeEdit->time().toString("hh:mm:ss");
    QString in = ui->timeEdit_2->time().toString("hh:mm:ss");
    int hour = qAbs(ui->timeEdit->time().hour()-ui->timeEdit_2->time().hour());
    int min = qAbs(ui->timeEdit->time().minute()-ui->timeEdit_2->time().minute());
    int sec = qAbs(ui->timeEdit->time().second()-ui->timeEdit_2->time().second());
    QString sub = QString::number(hour)+":"+QString::number(min)+":"+QString::number(sec);
    QString temp = ui->lineEdit_temp->text();
    QString normal = ui->comboBox->currentText();
    QString remark = ui->lineEdit_remark->text();

    if(id.isEmpty()||name.isEmpty()||temp.isEmpty())
    {
        ui->label_result->setText("信息不能为空");
        return;
    }

    QString str = "insert into `information` values ('"+id+"','"+name+"','"+out+"','"+in+"','"+sub+"','"+temp+"','"+normal+"','"+remark+"');";
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

void Admin_AddInfo::pushButton_back()
{
    ui->label_id->clear();
    ui->label_result->clear();
    ui->lineEdit_id->clear();
    ui->lineEdit_name->clear();
    ui->lineEdit_temp->clear();
    ui->lineEdit_remark->clear();
    ui->timeEdit->clear();
    ui->timeEdit_2->clear();
    this->close();
}

void Admin_AddInfo::checkId()
{
    QString id = ui->lineEdit_id->text();
    QSqlQuery query(db);
    QString str = "select 姓名 from `archive` where 学号='"+id+"';";
    query.exec(str);
    if(query.next())
    {
        if(query.value(0).toString()!=nullptr)
        {
            ui->lineEdit_name->setText(query.value(0).toString());
            ui->label_id->clear();
            return;
        }
    }
    ui->label_id->setText("学号不存在");
}

