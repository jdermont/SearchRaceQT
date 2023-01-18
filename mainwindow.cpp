#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(960,540);
    track = new Track(this);
    ui->gridLayout->addWidget(track);
    ui->gridLayout->addWidget(new QPushButton("test"));

//    MainWindow::setCentralWidget(track);
    track->setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

