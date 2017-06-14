#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    QPixmap *pix = new QPixmap(":/images/images/crm.png");
    splash->setPixmap(*pix);
    splash->show();
    splash->showMessage(QObject::tr("Загрузка."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    Sleep(1000);
    splash->showMessage(QObject::tr("Загрузка.."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    Sleep(1000);
    splash->showMessage(QObject::tr("Загрузка..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    Sleep(1000);
    MainWindow w;
    splash->showMessage(QObject::tr("Запуск"), Qt::AlignLeft | Qt::AlignBottom, Qt::yellow);
    w.show();
    splash->finish(&w);
    delete splash;

    return a.exec();
}
