#include "mainwindow.h"

#include <QApplication>

#include <authordialog.h>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  auto *w = new MainWindow;
  w->show();

  return a.exec();
}
