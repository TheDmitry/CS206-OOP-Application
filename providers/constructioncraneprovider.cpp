#include "constructioncraneprovider.h"
#include <QObject>

using namespace std;

ConstructionCraneProvider::ConstructionCraneProvider() {}

const std::string ConstructionCraneProvider::getName() const {
  return "ConstructionCraneProvider";
}

const std::string ConstructionCraneProvider::getItemName() const {
  return "ConstructionCrane";
}

const std::string ConstructionCraneProvider::getTrItemName() const {
  return QObject::tr("Construction Crane").toStdString();
}

const std::vector<std::string> ConstructionCraneProvider::getSupportedSchemeArgs() const {
  return {"modelName", "serial", "weight", "height", "angles", "position"};
}
const std::map<std::string, size_t> ConstructionCraneProvider::getScheme() const {
  return {{"modelName", 0},
          {"serial", 1},
          {"weight", 2},
          {"height", 3},
          {"angles", 4},
          {"position", 5}};
}

std::string ConstructionCraneProvider::get(std::shared_ptr<AbstractItem> const &target,
                                           std::string const &fieldName) const {
  auto i = getters.find(fieldName);

  if (i == getters.end()) {
    throw runtime_error("Unknown field: " + fieldName);
  }

  string field = i->second(static_cast<ConstructionCrane const &>(*target));

  return field;
}
void ConstructionCraneProvider::set(std::shared_ptr<AbstractItem> &target,
                                    std::string const &fieldName,
                                    std::string const &value) {
  auto i = setters.find(fieldName);

  if (i == setters.end()) {
    throw runtime_error("Unknown field: " + fieldName);
  }

  i->second(static_cast<ConstructionCrane &>(*target), value);
}

int ConstructionCraneProvider::compare(std::shared_ptr<AbstractItem> const &a,
                                       std::shared_ptr<AbstractItem> const &b,
                                       std::string const &fieldName) const {
  string aValue = get(a, fieldName);
  string bValue = get(b, fieldName);
  int retValue = 0;

  if (fieldName == "position" || fieldName == "angles") {
    Vector3 aVec = Vector3::vectorFromString(aValue);
    Vector3 bVec = Vector3::vectorFromString(bValue);

    if (aVec > bVec)
      retValue = 1;
    else if (aVec < bVec)
      retValue = -1;
  } else if (fieldName == "modelName" || fieldName == "serial") {
    retValue = aValue.compare(bValue);
  } else {
    int aI = stof(aValue);
    int bI = stof(bValue);

    if (aI > bI)
      retValue = 1;
    else if (aI < bI)
      retValue = -1;
  }

  return retValue;
}

std::shared_ptr<AbstractItem> ConstructionCraneProvider::create() {
  return make_shared<ConstructionCrane>();
}

// Should be optimized by RVO?
bool ConstructionCraneProvider::isChartSupported() const {
  return true;
}

const std::string ConstructionCraneProvider::getChartHeaderField() const {
  return "modelName";
}

const vector<string> ConstructionCraneProvider::getChartFields() const {
  return {"weight", "height"};
}
