#include "aunt.h"
#include "ui_aunt.h"

Aunt::Aunt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Aunt)
{
    ui->setupUi(this);

    connect(ui->pushButton_back,SIGNAL(clicked()),this,SLOT(PushbuttonBack()));//返回按钮
    //左侧按钮-------------------------------------------------
    ui->tabWidget->tabBar()->hide();//隐藏tab
    connect(ui->pushButton_record,SIGNAL(clicked()),this,SLOT(PushbuttonRecord()));
    connect(ui->pushButton_info,SIGNAL(clicked()),this,SLOT(PushbuttonInfo()));
    connect(ui->pushButton_data,SIGNAL(clicked()),this,SLOT(PushbuttonData()));
    connect(ui->pushButton_my,SIGNAL(clicked()),this,SLOT(PushbuttonMy()));
    connect(ui->pushButton_other,SIGNAL(clicked()),this,SLOT(PushbuttonOther()));
    //左侧按钮-------------------------------------------------
}

Aunt::~Aunt()
{
    delete ui;
}

//返回按键
void Aunt::PushbuttonBack()
{
    emit signal_BackWindow();
    this->close();
}

//-------------------------------------------------------------------------------------//
//按钮切换页面
void Aunt::PushbuttonRecord()
{
    ui->tabWidget->setCurrentIndex(0);
}

void Aunt::PushbuttonInfo()
{
    ui->tabWidget->setCurrentIndex(1);
}

void Aunt::PushbuttonData()
{
    ui->tabWidget->setCurrentIndex(2);
}

void Aunt::PushbuttonMy()
{
    ui->tabWidget->setCurrentIndex(3);
}

void Aunt::PushbuttonOther()
{
     ui->tabWidget->setCurrentIndex(4);
}

//-----------------------------------------------------------------------------------//

