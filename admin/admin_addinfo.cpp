#include "admin_addinfo.h"
#include "ui_admin_addinfo.h"
#include <QtSql/QSqlQuery>
#include <QKeyEvent>


Admin_AddInfo::Admin_AddInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Admin_AddInfo)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("mysql_connect");

    connect(ui->pushButton_confirm,SIGNAL(clicked()),this,SLOT(addInfo()));
    connect(ui->pushButton_quit,SIGNAL(clicked()),this,SLOT(pushButton_back()));
    connect(ui->lineEdit_id,SIGNAL(textChanged(QString)),this,SLOT(checkId(QString)));
    connect(ui->lineEdit_id,SIGNAL(textChanged (QString)),this,SLOT(showId(QString)));
    connect(ui->lineEdit_id,SIGNAL(editingFinished()),this,SLOT(listVisable()));
    connect(ui->listWidget,SIGNAL(itemSelectionChanged()),this,SLOT(mouseClicked()));
    ui->listWidget->setVisible(false);
}

Admin_AddInfo::~Admin_AddInfo()
{
    delete ui;
}

void Admin_AddInfo::addInfo()
{
    QString id = ui->lineEdit_id->text();
    QString name = ui->lineEdit_name->text();
    QString out = ui->timeEdit->time().toString("hh:mm:ss");
    QString in = ui->timeEdit_2->time().toString("hh:mm:ss");
    int hour = qAbs(ui->timeEdit->time().hour()-ui->timeEdit_2->time().hour());
    int min = qAbs(ui->timeEdit->time().minute()-ui->timeEdit_2->time().minute());
    int sec = qAbs(ui->timeEdit->time().second()-ui->timeEdit_2->time().second());
    QString sub = QString::number(hour)+":"+QString::number(min)+":"+QString::number(sec);
    QString temp = ui->lineEdit_temp->text();
    QString normal = ui->comboBox->currentText();
    QString remark = ui->lineEdit_remark->text();

    if(id.isEmpty()||name.isEmpty()||temp.isEmpty())
    {
        ui->label_result->setText("信息不能为空");
        return;
    }

    QString str = "insert into `information` values ('"+id+"','"+name+"','"+out+"','"+in+"','"+sub+"','"+temp+"','"+normal+"','"+remark+"');";
    QSqlQuery query(db);
    if(query.exec(str))
    {
        ui->label_result->setText("添加成功");
        emit refresh();
    }
    else {
        ui->label_result->setText("添加失败，请检查信息是否填写正确");
    }
}

void Admin_AddInfo::pushButton_back()
{
    ui->label_id->clear();
    ui->label_result->clear();
    ui->lineEdit_id->clear();
    ui->lineEdit_name->clear();
    ui->lineEdit_temp->clear();
    ui->lineEdit_remark->clear();
    ui->timeEdit->clear();
    ui->timeEdit_2->clear();
    this->close();
}

void Admin_AddInfo::showId(QString text)
{
    ui->listWidget->clear();
    ui->listWidget->setMinimumHeight(81);
    ui->listWidget->setMaximumHeight(81);
    QSqlQuery query(db);
    QString str = "select 学号 from `archive` where 学号 like '"+text+"%';";
    query.exec(str);
    QStringList header;
    while(query.next())
    {
        header << query.value(0).toString();
    }
    ui->listWidget->addItems(header);

    if(ui->listWidget->count()>0)
        ui->listWidget->setVisible(true);
    else {
         ui->listWidget->setVisible(false);
    }

    switch (ui->listWidget->count()) {
    case 1:
        ui->listWidget->setMinimumHeight(21);
        ui->listWidget->setMaximumHeight(21);
        break;
    case 2:
        ui->listWidget->setMinimumHeight(42);
        ui->listWidget->setMaximumHeight(42);
        break;
    case 3:
        ui->listWidget->setMinimumHeight(61);
        ui->listWidget->setMaximumHeight(61);
        break;
    }
}

void Admin_AddInfo::checkId(QString text)
{
    QSqlQuery query(db);
    QString str = "select 姓名 from `archive` where 学号='"+text+"';";
    query.exec(str);
    if(query.next())
    {
        if(query.value(0).toString()!=nullptr)
        {
            ui->lineEdit_name->setText(query.value(0).toString());
            ui->label_id->clear();
            return;
        }
    }
    ui->label_id->setText("学号不存在");
}

void Admin_AddInfo::mouseClicked()
{
    ui->lineEdit_id->setText(ui->listWidget->currentItem()->text());
    ui->listWidget->setVisible(false);
    ui->label_id->clear();
    checkId(ui->lineEdit_id->text());
}

void Admin_AddInfo::listVisable()
{
    ui->listWidget->setVisible(false);
}

void Admin_AddInfo::keyPressEvent ( QKeyEvent * keyevent )
{


    if(QApplication::focusWidget() != ui->lineEdit_id || ui->listWidget->count() == 0)return;
    int uKey = keyevent->key();
    Qt::Key key = static_cast<Qt::Key>(uKey);
    int iIndex = ui->listWidget->currentRow ();
    if(key == Qt::Key_Up)
    {
        iIndex--;
        if(iIndex < 0) iIndex = 0;
        QListWidgetItem *pItem = ui->listWidget->item(iIndex);
        ui->listWidget->setCurrentItem(pItem);

    }
    else if (key == Qt::Key_Down)
    {
        iIndex++;
        if(iIndex >= ui->listWidget->count()) iIndex = ui->listWidget->count() - 1;
        QListWidgetItem *pItem = ui->listWidget->item(iIndex);
        ui->listWidget->setCurrentItem(pItem);
    }
    else if (key == Qt::Key_Enter || key == Qt::Key_Return)
    {
        ui->lineEdit_id->setText(ui->listWidget->currentItem()->text());
        ui->listWidget->setVisible(false);
        ui->label_id->clear();
        checkId(ui->lineEdit_id->text());
    }
}

