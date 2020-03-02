#include "admin_modifymanager.h"
#include "ui_admin_modifymanager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

Admin_ModifyManager::Admin_ModifyManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_ModifyManager)
{
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(modifyManager()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(pushButton_back()));

}

Admin_ModifyManager::~Admin_ModifyManager()
{
    delete ui;
}

void Admin_ModifyManager::modifyManager()
{
    QString name = ui->lineEdit->text();
    QString id = ui->lineEdit_2->text();
    QString password = ui->lineEdit_3->text();
    if(name.isEmpty()||id.isEmpty()||password.isEmpty())
    {
        ui->label_result->setText("信息不能为空");
        return;
    }

    QString str = "update `manager` set 姓名='"+name+"',账号='"+id+"',密码='"+password+"' where 姓名='"+m_name+"' and 账号='"+m_id+"' and 密码='"+m_password+"';";
    QSqlDatabase db;
    db = QSqlDatabase::database("mysql_connect");
    QSqlQuery query(db);
    if(query.exec(str))
    {
        ui->label_result->setText("修改成功");
        emit refresh();
    }
    else {
        ui->label_result->setText("修改失败，请检查信息是否填写正确");
    }
}

void Admin_ModifyManager::pushButton_back()
{
    ui->label_result->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    this->close();
}

void Admin_ModifyManager::getManager(QString name, QString id, QString password)
{
    ui->lineEdit->setText(name);
     ui->lineEdit_2->setText(id);
      ui->lineEdit_3->setText(password);

      m_name = name;
      m_id = id;
      m_password = password;
}
