#ifndef AUNT_H
#define AUNT_H

#include <QWidget>

namespace Ui {
class Aunt;
}

class Aunt : public QWidget
{
    Q_OBJECT

public:
    explicit Aunt(QWidget *parent = nullptr);
    ~Aunt();

public slots:

    void PushbuttonBack();//返回按键
    //左边按钮
    void PushbuttonRecord();
    void PushbuttonInfo();
    void PushbuttonData();
    void PushbuttonMy();
    void PushbuttonOther();

signals:

    void signal_BackWindow();//窗口返回信号

private:
    Ui::Aunt *ui;
};

#endif // AUNT_H
