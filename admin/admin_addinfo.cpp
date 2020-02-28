#include "admin_addinfo.h"
#include "ui_admin_addinfo.h"

Admin_AddInfo::Admin_AddInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_AddInfo)
{
    ui->setupUi(this);
}

Admin_AddInfo::~Admin_AddInfo()
{
    delete ui;
}
