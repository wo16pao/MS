#include "exportthread.h"
#include <QDebug>
#include <QSqlQuery>
#include <QTime>

ExportThread::ExportThread()
{
    m_db = QSqlDatabase::database("mysql_connect");
    excelExport = new ExcelExport;
}

void ExportThread::getFilepath(QString filepath)
{
    m_filepath = filepath;
}

void ExportThread::getInfo(const QString &get_row, const QString &str, const QString &tableName, bool flag)
{
    m_get_row = get_row;
    m_str = str;
    m_tableName = tableName;
    m_flag = flag;
}

void ExportThread::run()
{
    excelExport->newExcel(m_filepath);

    int row=0;
    QSqlQuery query(m_db);
    QStringList header;
    QString head = "show columns from `"+m_tableName+"`;";
    query.exec(head);
    int col = 0;

    //将表头写入文件
    while (query.next()) {
        excelExport->setCellValue(1,col+1,query.value(0).toString());
        col++;
    }
    query.exec(m_get_row);
    if(query.first())
    {
        row=query.value("count(*)").toInt();//获取行数
    }
    if(row)
    {
        query.exec(m_str);
        for(int i=0; query.next() ;++i)
        {
            for(int j = 0; j<col ; ++j)
            {
                if(!m_flag)
                {
                    excelExport->setCellValue(i+2,j+1,query.value(j).toString());
                }
                else {
                    if(j==2||j==3)
                    {
                        QDateTime time;
                        time = query.value(j).toDateTime();
                        QString strBuffer;
                        strBuffer = time.toString("yyyy-MM-dd hh:mm:ss");
                        excelExport->setCellValue(i+2,j+1,strBuffer);

                    }
                    else {
                        excelExport->setCellValue(i+2,j+1,query.value(j).toString());
                    }
                }
            }
        }
    }

    excelExport->saveExcel(m_filepath);

    emit finish();
}
