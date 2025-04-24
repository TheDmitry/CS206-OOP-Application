#include <QApplication>
#include "controllers/mainwindow.h"
#include "providers/vrheadsetprovider.h"
#include <memory>

using namespace std;

int main(int argc, char *argv[]) {
  shared_ptr<AbstractProvider> vrProvider = make_shared<VRHeadsetProvider>();
  DbFile::registerProvider(vrProvider->getName(), vrProvider);

  QApplication a(argc, argv);

  auto *w = new MainWindow;
  w->show();

  return a.exec();
}
