#include <QApplication>
#include <mainwindow.h>
#include <QRect>

#if QT_VERSION >= 0x050000
#include <QWidget>
#include <QtWidgets>
#else
#include <QApplication>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

#if QT_VERSION >= 0x050000
    QDesktopWidget *b = new QDesktopWidget();
    QRect screenGeometry = b->screenGeometry(&w);
    b->deleteLater();
#else
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
#endif



    int x = (screenGeometry.width()-w.width()) / 2;
    int y = (screenGeometry.height()-w.height()) / 2;
    w.move(x, y);
    w.show();
    return a.exec();
}
