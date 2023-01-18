#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    srand(time(NULL) ^ uint64_t(&main));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
