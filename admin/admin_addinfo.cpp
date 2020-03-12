#include "admin_addinfo.h"
#include "ui_admin_addinfo.h"
#include <QtSql/QSqlQuery>
#include <QKeyEvent>
#include <QDateTime>
#include <QDate>

Admin_AddInfo::Admin_AddInfo(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::Admin_AddInfo)
{
    initTitleBar();
    ui->setupUi(this);
    db = QSqlDatabase::database("mysql_connect");
    ui->listWidget->setMouseTracking(true);

    connect(ui->pushButton_confirm,SIGNAL(clicked()),this,SLOT(addInfo()));
    connect(ui->pushButton_quit,SIGNAL(clicked()),this,SLOT(pushButton_back()));
    connect(ui->lineEdit_id,SIGNAL(textChanged(QString)),this,SLOT(checkId(QString)));
    connect(ui->lineEdit_id,SIGNAL(textChanged (QString)),this,SLOT(showId(QString)));
    connect(ui->lineEdit_id,SIGNAL(editingFinished()),this,SLOT(listVisable()));
    connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(mouseClicked(QListWidgetItem*)));
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
//    QString out = ui->timeEdit->time().toString("hh:mm:ss");
//    QString in = ui->timeEdit_2->time().toString("hh:mm:ss");
//    int hour = qAbs(ui->timeEdit->time().hour()-ui->timeEdit_2->time().hour());
//    int min = qAbs(ui->timeEdit->time().minute()-ui->timeEdit_2->time().minute());
//    int sec = qAbs(ui->timeEdit->time().second()-ui->timeEdit_2->time().second());
    QString out = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString in = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss");


    QString sub = QDateTime::fromSecsSinceEpoch(ui->dateTimeEdit_2->dateTime().toSecsSinceEpoch()-ui->dateTimeEdit->dateTime().toSecsSinceEpoch()).toUTC().toString("hh:mm:ss");

    //QString sub = time;
    QString temp = ui->lineEdit_temp->text();
    QString normal = ui->comboBox->currentText();
    QString remark = ui->lineEdit_remark->text();

    if(id.isEmpty()||name.isEmpty()||temp.isEmpty())
    {
        ui->label_result->setText("信息不能为空");
        return;
    }

    QString str = "insert into `information` select '"+id+"','"+name+"','"+out+"','"+in+"','"+sub+"','"+temp+"','"+normal+"','"+remark+"' from dual where EXISTS(SELECT *FROM archive WHERE 学号='"+id+"' and 姓名='"+name+"');";
    QSqlQuery query(db);
    if(query.exec(str))
    {
        if(query.numRowsAffected()!=-1&&query.numRowsAffected()!=0)
        {
            ui->label_result->setText("添加成功");
            emit refresh();
        }
        else {
            ui->label_result->setText("添加失败");
        }
    }
    else {
        ui->label_result->setText("添加失败");
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
    ui->dateTimeEdit->clear();
    ui->dateTimeEdit_2->clear();
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

void Admin_AddInfo::mouseClicked(QListWidgetItem* item)
{
    ui->lineEdit_id->setText(item->text());
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

void Admin_AddInfo::initTitleBar()
{
    // 设置标题栏跑马灯效果，可以不设置;
    //m_titleBar->setTitleRoll();
    m_titleBar->setBackgroundColor(56,70,85);
    m_titleBar->setTitleIcon(":/icon.png");
    m_titleBar->setTitleContent(QStringLiteral("我的窗口"));
    m_titleBar->setButtonType(MIN_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
