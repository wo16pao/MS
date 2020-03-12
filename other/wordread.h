#ifndef WORDREAD_H
#define WORDREAD_H
#include <ActiveQt/QAxObject>

class WordRead
{
public:
    WordRead();

    QString readContent(const QString& filepath);
};

#endif // WORDREAD_H
