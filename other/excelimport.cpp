#include "excelimport.h"
#include <QDebug>
#include <QTime>
#include <QSqlQuery>
#include <QVector>

ExcelImport::ExcelImport()
{
    db = QSqlDatabase::database("mysql_connect");

}

void ExcelImport::importInfo(QString filepath)
{
    QString id,name,out,in,sub,temp,normal,remark;

    m_success = 0;
    m_failure = 0;

    QAxObject* excel = new QAxObject("Excel.Application");
    excel->setProperty("Visible", false);
    QAxObject* workbooks = excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)", filepath); //filepath
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1); //worksheet number
    QAxObject* usedrange = worksheet->querySubObject("UsedRange");
    QAxObject* rows = usedrange->querySubObject("Rows");
    QAxObject* columns = usedrange->querySubObject("Columns");
    int intRowStart = usedrange->property("Row").toInt();
    int intColStart = usedrange->property("Column").toInt();
    int intRows = rows->property("Count").toInt();
    int intCols = columns->property("Count").toInt();
    QAxObject * cell;
    QVariant cellValue;
    for (int i = intRowStart+1; i < intRowStart + intRows; i++)
    {
        QVector<QString> vStr;


        for (int j = intColStart; j < intColStart + intCols; j++)
        {
            cell = excel->querySubObject("Cells(Int, Int)", i, j );
            QVariant cellValue = cell->dynamicCall("Text");
            if(j==3||j==4)
            {
                QString temp = cellValue.toString();
                temp.replace(4,1,"-");
                temp.replace(6,1,"-");
                vStr.push_back(temp);
            }
            else {
                vStr.push_back(cellValue.toString());
            }
        }


        QString str = "insert into `information` values (";
        QString str1 = nullptr;
        for(int k = 0 ; k<vStr.size();++k)
        {
            str1 += "'"+vStr[k]+"',";
        }
        str1.replace(str1.size()-1,1,")");
        str += str1;
        QSqlQuery query(db);
        if(query.exec(str))
        {
            ++m_success;
        }
        else {
            ++m_failure;
        }

    }
    excel->setProperty("DisplayAlerts", 0);
    workbook->dynamicCall("Save(void)");
    workbook->dynamicCall("Close (Boolean)", false);
    excel->setProperty("DisplayAlerts",1);
    excel->dynamicCall("Quit(void)");
    delete excel;

}

void ExcelImport::importArchive(QString filepath)
{
    importFunction(filepath,"archive");
}

void ExcelImport::importDormitory(QString filepath)
{
    importFunction(filepath,"domitory");
}

void ExcelImport::importDean(QString filepath)
{
    importFunction(filepath,"dean");
}

void ExcelImport::importAunt(QString filepath)
{
    importFunction(filepath,"aunt");
}

void ExcelImport::importManager(QString filepath)
{
    importFunction(filepath,"manager");
}

void ExcelImport::importFunction(QString filepath, QString tableName)
{
    m_success = 0;
    m_failure = 0;

    QAxObject* excel = new QAxObject("Excel.Application");
    excel->setProperty("Visible", false);
    QAxObject* workbooks = excel->querySubObject("WorkBooks");
    workbooks->dynamicCall("Open (const QString&)", filepath); //filepath
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
    QAxObject* worksheet = workbook->querySubObject("Worksheets(int)", 1); //worksheet number
    QAxObject* usedrange = worksheet->querySubObject("UsedRange");
    QAxObject* rows = usedrange->querySubObject("Rows");
    QAxObject* columns = usedrange->querySubObject("Columns");
    int intRowStart = usedrange->property("Row").toInt();
    int intColStart = usedrange->property("Column").toInt();
    int intRows = rows->property("Count").toInt();
    int intCols = columns->property("Count").toInt();
    QAxObject * cell;
    QVariant cellValue;
    for (int i = intRowStart+1; i < intRowStart + intRows; i++)
    {
        QVector<QString> vStr;
        for (int j = intColStart; j < intColStart + intCols; j++)
        {
            cell = excel->querySubObject("Cells(Int, Int)", i, j );
            QVariant cellValue = cell->dynamicCall("Text");
            vStr.push_back(cellValue.toString());

        }

        QString str = "insert into `"+tableName+"` values (";
        QString str1 = nullptr;
        for(int k = 0 ; k<vStr.size();++k)
        {
            str1 += "'"+vStr[k]+"',";
        }
        str1.replace(str1.size()-1,1,")");
        str += str1;
        QSqlQuery query(db);
        if(query.exec(str))
        {
            ++m_success;
        }
        else {
            ++m_failure;
        }
    }
    excel->setProperty("DisplayAlerts", 0);
    workbook->dynamicCall("Save(void)");
    workbook->dynamicCall("Close (Boolean)", false);
    excel->setProperty("DisplayAlerts",1);
    excel->dynamicCall("Quit(void)");
    delete excel;
}

int ExcelImport::getSuccess()
{
    return m_success;
}

int ExcelImport::getFailure()
{
    return m_failure;
}
