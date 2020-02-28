#include "admin_adddean.h"
#include "ui_admin_adddean.h"

Admin_AddDean::Admin_AddDean(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_AddDean)
{
    ui->setupUi(this);
}

Admin_AddDean::~Admin_AddDean()
{
    delete ui;
}
