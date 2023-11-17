#include "HRM_GUI.h"

#include <QApplication>
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HRM_GUI w;
    w.show();
    return a.exec();
}