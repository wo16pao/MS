#include "admin_adddormitory.h"
#include "ui_admin_adddormitory.h"

Admin_AddDormitory::Admin_AddDormitory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_AddDormitory)
{
    ui->setupUi(this);
}

Admin_AddDormitory::~Admin_AddDormitory()
{
    delete ui;
}
