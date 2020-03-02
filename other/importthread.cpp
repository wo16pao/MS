#include "importthread.h"

ImportThread::ImportThread()
{
    excelImport = new ExcelImport;
}

void ImportThread::run()
{
    switch (m_index) {
    case 1:
        excelImport->importInfo(m_filepath);
        break;
    case 2:
        excelImport->importArchive(m_filepath);
        break;
    case 3:
        excelImport->importDormitory(m_filepath);
        break;
    case 4:
        excelImport->importDean(m_filepath);
        break;
    case 5:
        excelImport->importAunt(m_filepath);
        break;
    case 6:
        excelImport->importManager(m_filepath);
    }

    m_success = excelImport->getSuccess();
    m_failure = excelImport->getFailure();

    emit finish(m_success,m_failure);
}

void ImportThread::getFilepath(QString filepath)
{
    m_filepath = filepath;
}

void ImportThread::getIndex(int index)
{
    m_index = index;
}


