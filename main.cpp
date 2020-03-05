#include "widget.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
//    //加载样式表
//    //QFile file(":/qss/psblack.css");
//    QFile file(":/qss/flatwhite.css");
//    //QFile file(":/qss/lightblue.css");
//    if (file.open(QFile::ReadOnly)) {
//        QString qss = QLatin1String(file.readAll());
//        QString paletteColor = qss.mid(20, 7);
//        qApp->setPalette(QPalette(QColor(paletteColor)));
//        qApp->setStyleSheet(qss);
//        file.close();
//    }

    w.show();

    return a.exec();
}
