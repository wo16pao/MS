#include "admin_addmanager.h"
#include "ui_admin_addmanager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

Admin_AddManager::Admin_AddManager(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Admin_AddManager)
{
    initTitleBar();
    ui->setupUi(this);
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(addManager()));
        connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(pushButton_back()));

}

Admin_AddManager::~Admin_AddManager()
{
    delete ui;
}

void Admin_AddManager::addManager()
{
    QString name = ui->lineEdit->text();
    QString id = ui->lineEdit_2->text();
    QString password = ui->lineEdit_3->text();

    if(name.isEmpty()||id.isEmpty()||password.isEmpty())
    {
        ui->label_result->setText("信息不能为空");
        return;
    }

    QString str = "insert into `manager` values ('"+name+"','"+id+"','"+password+"');";
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

void Admin_AddManager::pushButton_back()
{
    ui->label_result->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    this->close();
}

void Admin_AddManager::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(56,70,85);
    m_titleBar->setTitleIcon(":/icon.png");
    m_titleBar->setTitleContent(QStringLiteral("我的窗口"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
