#include "wordread.h"

WordRead::WordRead()
{

}

QString WordRead::readContent(const QString& filepath)
{
    QAxObject *word = new QAxObject("Word.Application");
    word->setProperty("Visible", false);
    QAxObject *documents = word->querySubObject("Documents");
    documents->dynamicCall("Open (const QString&)",filepath);
    QAxObject * document = word->querySubObject("ActiveDocument");
    QAxObject *range=document->querySubObject("Range()");
    QString text = range->property("Text").toString();
    document->dynamicCall("Close (Boolean)", true);
    word->dynamicCall("Quit (void)");
    return text;
}
