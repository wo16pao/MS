#include "admin_addaunt.h"
#include "ui_admin_addaunt.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "other/md5.h"

Admin_AddAunt::Admin_AddAunt(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Admin_AddAunt)
{
    initTitleBar();
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

    string stdPassword = password.toStdString();
    stdPassword = MD5(stdPassword).toStr();
    password = QString::fromStdString(stdPassword);

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
        ui->label_result->setText("添加失败");
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

void Admin_AddAunt::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(56,70,85);
    m_titleBar->setTitleIcon(":/icon.png");
    m_titleBar->setTitleContent(QStringLiteral("我的窗口"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
