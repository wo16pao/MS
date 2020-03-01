#ifndef IMPORTTHREAD_H
#define IMPORTTHREAD_H
#include <QThread>
#include "excelimport.h"

class ImportThread : public QThread
{
    Q_OBJECT
public:
    ImportThread();

protected:
    void run() Q_DECL_OVERRIDE;
public:
    void getFilepath(QString);
    void getIndex(int index);

signals:
    void finish(int success,int failure);
private:
    ExcelImport *excelImport;
    QString m_filepath;
    int m_index;

    int m_success;
    int m_failure;
};

#endif // IMPORTTHREAD_H
