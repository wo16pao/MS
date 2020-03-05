#include "admin_modifydormitory.h"
#include "ui_admin_modifydormitory.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

Admin_ModifyDormitory::Admin_ModifyDormitory(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Admin_ModifyDormitory)
{
    initTitleBar();
    ui->setupUi(this);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(modifyDormitory()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(pushButton_back()));
}

Admin_ModifyDormitory::~Admin_ModifyDormitory()
{
    delete ui;
}

void Admin_ModifyDormitory::pushButton_back()
{
    ui->label_result->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    this->close();
}

void Admin_ModifyDormitory::modifyDormitory()
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

    QString str = "update `dormitory` set 宿舍区域='"+area+"',楼栋号='"+building+"',大寝号='"+big+"',小寝号='"+small+"',床号='"+bed+"' where 宿舍区域='"+m_area+"' and 楼栋号='"+m_building+"' and 大寝号='"+m_big+"' and 小寝号='"+m_small+"' and 床号='"+m_bed+"';";
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

void Admin_ModifyDormitory::getDormitory(QString area, QString building , QString big, QString small, QString bed)
{
    ui->comboBox->setCurrentText(area);
    ui->lineEdit->setText(building);
    ui->lineEdit_2->setText(big);
    ui->lineEdit_3->setText(small);
    ui->lineEdit_4->setText(bed);
    m_area = area;
    m_building = building;
    m_big = big;
    m_small = small;
    m_bed = bed;
}

void Admin_ModifyDormitory::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(56,70,85);
    m_titleBar->setTitleIcon(":/icon.png");
    m_titleBar->setTitleContent(QStringLiteral("我的窗口"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
