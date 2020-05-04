#include "admin_adddormitory.h"
#include "ui_admin_adddormitory.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

Admin_AddDormitory::Admin_AddDormitory(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Admin_AddDormitory)
{
    initTitleBar();
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(addDormitory()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(pushButton_back()));
}

Admin_AddDormitory::~Admin_AddDormitory()
{
    delete ui;
}

void Admin_AddDormitory::pushButton_back()
{
    ui->label_result->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    this->close();
}

void Admin_AddDormitory::addDormitory()
{
    QString area = ui->comboBox->currentText();
    QString building = ui->lineEdit->text();
    QString big = ui->lineEdit_2->text();
    QString small = ui->lineEdit_3->text();
    QString bed = ui->lineEdit_4->text();
    if(building.isEmpty()||big.isEmpty()||small.isEmpty()||bed.isEmpty())
    {
        ui->label_result->setText("信息不能为空");
        return;
    }

    QString str = "insert into `dormitory` values ('"+area+"','"+building+"','"+big+"','"+small+"','"+bed+"');";
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

void Admin_AddDormitory::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(56,70,85);
    m_titleBar->setTitleIcon(":/icon3.png");
    m_titleBar->setTitleContent(QStringLiteral("学生体温监控管理系统"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}

