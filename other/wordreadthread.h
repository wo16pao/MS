#ifndef WORDREADTHREAD_H
#define WORDREADTHREAD_H
#include <QThread>
#include "wordread.h"

class WordReadThread : public QThread
{
    Q_OBJECT
public:
    WordReadThread();
protected:
    void run() Q_DECL_OVERRIDE;
public:
    void getFilepath(const QString&);

signals:
    void finish(const QString& text);

private:
    WordRead* m_wordRead;
    QString m_filepath;
    QString m_text;
};

#endif // WORDREADTHREAD_H
