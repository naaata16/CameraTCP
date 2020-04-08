#include "mainwindow.h"
#include <QApplication>
#include "client.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyClient client("petrov.in.ua:185", 80);

    client.show();

    return a.exec();
}
