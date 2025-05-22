#include <QApplication>
#include "external/dbfile.h"
#include "providers/abstractprovider.h"
#include "providers/vrheadsetprovider.h"
#include "providers/constructioncraneprovider.h"
#include "controllers/mainwindow.h"
#include "providers/constructioncraneprovider.h"
#include "providers/vrheadsetprovider.h"
#include <memory>

using namespace std;

int main(int argc, char *argv[]) {
  unique_ptr<AbstractProvider> vrProvider = make_unique<VRHeadsetProvider>();
  unique_ptr<AbstractProvider> ccProvider = make_unique<ConstructionCraneProvider>();

  DbFile::registerProvider(vrProvider->getName(), std::move(vrProvider));
  DbFile::registerProvider(ccProvider->getName(), std::move(ccProvider));

  QApplication a(argc, argv);

  MainWindow w;
  w.show();

  return a.exec();
}
