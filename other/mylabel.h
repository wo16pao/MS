#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QMouseEvent>


class MyLabel : public QLabel
{
     Q_OBJECT
public:

    explicit MyLabel(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event);  //抬起

signals:
    void clicked(const QString&);  //抬起


};

#endif // MYLABEL_H
