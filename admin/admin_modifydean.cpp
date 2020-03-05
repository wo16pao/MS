#include "admin_modifydean.h"
#include "ui_admin_modifydean.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

Admin_ModifyDean::Admin_ModifyDean(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Admin_ModifyDean)
{
    initTitleBar();
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(modifyDean()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(pushButton_back()));
}

Admin_ModifyDean::~Admin_ModifyDean()
{
    delete ui;
}

void Admin_ModifyDean::modifyDean()
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

    QString str = "update `dean` set 学院名称='"+name+"',专业名称='"+major+"',班级号='"+classId+"',辅导员='"+teacher+"' where 学院名称='"+m_name+"' and 专业名称='"+m_major+"' and 班级号='"+m_classId+"' and 辅导员='"+m_teacher+"';";
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

void Admin_ModifyDean::pushButton_back()
{
    ui->label_result->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    this->close();
}

void Admin_ModifyDean::getDean(QString name, QString major, QString classId, QString teacher)
{
    ui->lineEdit->setText(name);
    ui->lineEdit_2->setText(major);
    ui->lineEdit_3->setText(classId);
    ui->lineEdit_4->setText(teacher);
    m_name = name;
    m_major = major;
    m_classId = classId;
    m_teacher = teacher;
}

void Admin_ModifyDean::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(56,70,85);
    m_titleBar->setTitleIcon(":/icon.png");
    m_titleBar->setTitleContent(QStringLiteral("我的窗口"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
