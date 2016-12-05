#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QDesktopWidget>

int main(int argc, char *argv[]) {

    QApplication::setApplicationDisplayName("Mercury");
    QApplication a(argc, argv);

    MainWindow w;

    w.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            w.size(),
            a.desktop()->availableGeometry()
        )
    );

    return a.exec();
}
