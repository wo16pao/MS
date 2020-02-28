#include "admin_addarchive.h"
#include "ui_admin_addarchive.h"

Admin_AddArchive::Admin_AddArchive(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_AddArchive)
{
    ui->setupUi(this);
}

Admin_AddArchive::~Admin_AddArchive()
{
    delete ui;
}
