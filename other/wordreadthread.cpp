#include "wordreadthread.h"

WordReadThread::WordReadThread()
{
    m_wordRead = new WordRead;
}

void WordReadThread::run()
{
    m_text = m_wordRead->readContent(m_filepath);

    delete m_wordRead;
    emit finish(m_text);
}

void WordReadThread::getFilepath(const QString &filepath)
{
    m_filepath = filepath;
}
