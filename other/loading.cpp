#include "loading.h"
#include "ui_loading.h"
#include <QDesktopWidget>

Loading::Loading(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Loading)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);//背景透明
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);//设置为对话框风格，并且去掉边框
    setWindowModality(Qt::WindowModal);//设置为模式对话框，同时在构造该对话框时要设置父窗口

    m_movie = new QMovie(":/res/loading.gif");
    ui->label->setMovie(m_movie);
    ui->label->setScaledContents(true);
    m_movie->start();
}

Loading::~Loading()
{
    delete ui;
    m_movie->stop();
    delete m_movie;
}
