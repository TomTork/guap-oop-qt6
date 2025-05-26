#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(600, 500);
    w.setWindowTitle("Ремонтная мастерская телефонов — Саиткулов Дмитрий 4332");
    w.show();
    return QApplication::exec();
}