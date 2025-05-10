#include <cstdlib>

#include <QObject>
#include "vrheadsetprovider.h"

using namespace std;

VRHeadsetProvider::VRHeadsetProvider() {}

const string VRHeadsetProvider::getName() const {
  return "VRHeadsetProvider";
};

const std::string VRHeadsetProvider::getItemName() const {
  return "VRHeadset";
}

const std::string VRHeadsetProvider::getTrItemName() const {
  return QObject::tr("VR Headset").toStdString();
}

const vector<string> VRHeadsetProvider::getSupportedSchemeArgs() const {
  return {"modelName", "width", "height", "refreshRate", "angles", "position"};
};

const map<string, size_t> VRHeadsetProvider::getScheme() const {
  return {{"modelName", 0},
          {"width", 1},
          {"height", 2},
          {"refreshRate", 3},
          {"angles", 4},
          {"position", 5}};
};

string VRHeadsetProvider::get(shared_ptr<AbstractItem> const &target,
                              string const &fieldName) const {
  auto i = getters.find(fieldName);

  if (i == getters.end()) {
    throw runtime_error("Unknown field: " + fieldName);
  }

  string field = i->second(static_cast<VRHeadset const &>(*target));

  return field;
}

void VRHeadsetProvider::set(shared_ptr<AbstractItem> &target,
                            string const &fieldName,
                            string const &value) {
  auto i = setters.find(fieldName);

  if (i == setters.end()) {
    throw runtime_error("Unknown field: " + fieldName);
  }

  i->second(static_cast<VRHeadset &>(*target), value);
}

int VRHeadsetProvider::compare(shared_ptr<AbstractItem> const &a,
                               const shared_ptr<AbstractItem> &b,
                               string const &fieldName) const {
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
  } else if (fieldName == "modelName") {
    retValue = aValue.compare(bValue);
  } else if (fieldName == "refreshRate") {
    float aF = stof(aValue);
    float bF = stof(bValue);

    if (aF > bF)
      retValue = 1;
    else if (aF < bF)
      retValue = -1;
  } else {
    int aI = stoi(aValue);
    int bI = stoi(bValue);

    if (aI > bI)
      retValue = 1;
    else if (aI < bI)
      retValue = -1;
  }

  return retValue;
}

shared_ptr<AbstractItem> VRHeadsetProvider::create() {
  return make_shared<VRHeadset>();
}

bool VRHeadsetProvider::isChartSupported() const {
  return true;
}

const string VRHeadsetProvider::getChartHeaderField() const {
  return "modelName";
}

const vector<string> VRHeadsetProvider::getChartFields() const {
  return {"width", "height", "refreshRate"};
}
