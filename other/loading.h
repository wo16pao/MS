#ifndef LOADING_H
#define LOADING_H

#include <QWidget>
#include <QMovie>

namespace Ui {
class Loading;
}

class Loading : public QWidget
{
    Q_OBJECT

public:
    explicit Loading(QWidget *parent = nullptr);
    ~Loading();

private:
    Ui::Loading *ui;

    QMovie *m_movie;
};

#endif // LOADING_H
