#include "mainwindow.h"

#include <QApplication>
#include "playerwindow.h"
#include <QtAVWidgets>


int main(int argc, char *argv[])
{
  int result = 0;
  do
  {
    QtAV::Widgets::registerRenderers();
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Stream Service");
    w.show();
    result=a.exec();
  } while( result == 1234 );


}
