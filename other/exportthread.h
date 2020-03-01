#ifndef EXPORTTHREAD_H
#define EXPORTTHREAD_H
#include <QThread>
#include <QSqlDatabase>
#include "excelexport.h"

class ExportThread : public QThread
{
    Q_OBJECT
public:
    ExportThread();
protected:
    void run() Q_DECL_OVERRIDE;
public:
    void getFilepath(QString);

    void getInfo(const QString &row ,const QString &str,const QString &tableName,bool flag = false);
signals:
    void finish();
private:
    ExcelExport *excelExport;
    QString m_filepath;

    QSqlDatabase m_db;

    QString m_get_row;
    QString m_str;
    QString m_tableName;
    bool m_flag;
};


#endif // EXPORTTHREAD_H
