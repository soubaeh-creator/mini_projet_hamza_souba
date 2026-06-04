#include <QApplication>
#include <QDir>
#include "gui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QDir().mkpath("data");
    MainWindow w;
    w.show();
    return a.exec();
}