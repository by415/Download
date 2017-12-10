#include "downloadTools.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    downloadTools w;
    w.show();

    return a.exec();
}
