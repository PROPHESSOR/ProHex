#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    if(argc == 2) {
        QString arg(argv[1]);

        if(arg == "-v" || arg == "--version") {
            qDebug() << "" + PROHEX + " v." + PROHEX_VERSION;
            qDebug() << "Copyright (c) PROPHESSOR 2018";
            return 0;
        } else if(arg == "-h" || arg == "--help") {
            qDebug() << "" + PROHEX + " v." + PROHEX_VERSION;
            qDebug() << "Copyright (c) PROPHESSOR 2018";
            qDebug() << "=============================";
            qDebug() << "You can get help using F1 key in program!";
            return 0;
        }
    }

    QApplication a(argc, argv);

    MainWindow w(nullptr);

    w.show();

    return a.exec();
}
