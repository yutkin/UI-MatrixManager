#include "FrontView.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FrontView w;
    w.show();
    return a.exec();
}
