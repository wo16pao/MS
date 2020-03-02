#ifndef EXCELIMPORT_H
#define EXCELIMPORT_H
#include <ActiveQt/QAxObject>
#include <QSqlDatabase>

class ExcelImport
{
public:
    ExcelImport();

    void importInfo(QString);
    void importArchive(QString);
    void importDormitory(QString);
    void importDean(QString);
    void importAunt(QString);
    void importManager(QString);

    void importFunction(QString filepath,QString tableName);

    int getSuccess();
    int getFailure();

signals:
    void finish(int success,int failure);

private:
    QSqlDatabase db;
    int m_success;
    int m_failure;
};

#endif // EXCELIMPORT_H
