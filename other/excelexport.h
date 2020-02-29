#ifndef EXCELEXPORT_H
#define EXCELEXPORT_H
#include<ActiveQt/QAxObject>
#include <QDir>
#include <QFile>
#include <qstring.h>

class ExcelExport
{
public:
    ExcelExport();
    ~ExcelExport();

    void newExcel(const QString &fileName);

    void setCellValue(int row, int column, const QString &value);

    void saveExcel(const QString &fileName);


    QAxObject *pApplication;
    QAxObject *pWorkBooks;
    QAxObject *pWorkBook;
    QAxObject *pSheets;
    QAxObject *pSheet;

};

#endif // EXCELEXPORT_H
