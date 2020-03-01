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
    int intRowStart = usedrange->property("Row").toInt();
    int intRows = rows->property("Count").toInt();
    QAxObject * cell;
    QVariant cellValue;
    for (int i = intRowStart+1; i < intRowStart + intRows; i++)
    {
        int j = 1;

        cell = excel->querySubObject("Cells(int, int)", i, j++ );
        cellValue = cell->dynamicCall("Text");
        id = cellValue.toString();

        cell = excel->querySubObject("Cells(int, int)", i, j++ );
        cellValue = cell->dynamicCall("Text");
        name = cellValue.toString();

        cell = excel->querySubObject("Cells(int, int)", i, j++ );
        cellValue = cell->dynamicCall("Text");
        out = cellValue.toString();

        cell = excel->querySubObject("Cells(int, int)", i, j++ );
        cellValue = cell->dynamicCall("Text");
        in = cellValue.toString();

        cell = excel->querySubObject("Cells(int, int)", i, j++ );
        cellValue = cell->dynamicCall("Text");
        sub = cellValue.toString();

        cell = excel->querySubObject("Cells(int, int)", i, j++ );
        cellValue = cell->dynamicCall("Text");
        temp = cellValue.toString();

        cell = excel->querySubObject("Cells(int, int)", i, j++ );
        cellValue = cell->dynamicCall("Text");
        normal = cellValue.toString();

        cell = excel->querySubObject("Cells(int, int)", i, j++ );
        cellValue = cell->dynamicCall("Text");
        remark = cellValue.toString();


        QSqlQuery  query(db);

        QString str = "insert into `information` select '"+id+"','"+name+"','"+out+"','"+in+"','"+sub+"','"+temp+"','"+normal+"','"+remark+"' from dual where EXISTS(SELECT *FROM archive WHERE 学号='"+id+"' and 姓名='"+name+"');";

        if(query.exec(str))
        {
            if(query.numRowsAffected()!=-1&&query.numRowsAffected()!=0)
            {
                ++m_success;
            }
            else {
                ++m_failure;
            }
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
            if(j==5||j==7)
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

        QString str = "insert into `archive` values (";
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

void ExcelImport::importDormitory(QString filepath)
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

        QString str = "insert into `dormitory` values (";
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

void ExcelImport::importDean(QString filepath)
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

        QString str = "insert into `dean` values (";
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
