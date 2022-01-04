#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QUuid>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //--------open style file
    QFile styleF(":/style.css");
    //styleF.setFileName(QString( QCoreApplication::applicationDirPath())+"/style.css");
    styleF.open(QFile::ReadOnly | QIODevice::Text);
    QString qssStr = styleF.readAll();
    qApp->setStyleSheet(qssStr);
    //----------------------------------------------

    w.show();
    return a.exec();
}
