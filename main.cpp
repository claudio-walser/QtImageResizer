#include "qtimageresizer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtImageResizer w;
    w.show();

    return a.exec();
}
