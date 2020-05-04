#include "admin_modifyaunt.h"
#include "ui_admin_modifyaunt.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "other/md5.h"

Admin_ModifyAunt::Admin_ModifyAunt(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Admin_ModifyAunt)
{
    initTitleBar();
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(modifyAunt()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(pushButton_back()));

}

Admin_ModifyAunt::~Admin_ModifyAunt()
{
    delete ui;
}

void Admin_ModifyAunt::modifyAunt()
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
    string stdPassword = password.toStdString();
    stdPassword = MD5(stdPassword).toStr();
    password = QString::fromStdString(stdPassword);

    QString str = "update `aunt` set 姓名='"+name+"',账号='"+id+"',密码='"+password+"',宿舍区域='"+area+"' where 姓名='"+m_name+"' and 账号='"+m_id+"';";
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

void Admin_ModifyAunt::pushButton_back()
{
    ui->label_result->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    this->close();
}

void Admin_ModifyAunt::getAunt(QString name, QString id, QString password, QString area)
{
    ui->lineEdit->setText(name);
     ui->lineEdit_2->setText(id);
      ui->lineEdit_3->setText(password);
      ui->comboBox->setCurrentText(area);
      m_name = name;
      m_id = id;
      m_password = password;
      m_area = area;
}

void Admin_ModifyAunt::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(56,70,85);
    m_titleBar->setTitleIcon(":/icon3.png");
    m_titleBar->setTitleContent(QStringLiteral("学生体温监控管理系统"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}

