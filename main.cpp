#include <QApplication>
#include "controllers/mainwindow.h"
#include "providers/vrheadsetprovider.h"
#include <memory>

using namespace std;

int main(int argc, char *argv[]) {
  unique_ptr<AbstractProvider> vrProvider = make_unique<VRHeadsetProvider>();
  DbFile::registerProvider(vrProvider->getName(), std::move(vrProvider));

  QApplication a(argc, argv);

  auto *w = new MainWindow;
  w->show();

  return a.exec();
}
