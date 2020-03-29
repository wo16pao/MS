#include "admin_modifymanager.h"
#include "ui_admin_modifymanager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "other/md5.h"

Admin_ModifyManager::Admin_ModifyManager(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Admin_ModifyManager)
{
    initTitleBar();
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
    string stdPassword = password.toStdString();
    stdPassword = MD5(stdPassword).toStr();
    password = QString::fromStdString(stdPassword);
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
        ui->label_result->setText("修改失败");
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

void Admin_ModifyManager::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(56,70,85);
    m_titleBar->setTitleIcon(":/icon.png");
    m_titleBar->setTitleContent(QStringLiteral("我的窗口"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
