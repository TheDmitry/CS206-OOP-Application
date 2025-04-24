#include "vrheadsetprovider.h"
#include <memory>

VRHeadsetProvider::VRHeadsetProvider() {}

const string VRHeadsetProvider::getName() {
  return "VRHeadsetProvider";
};

const vector<string> VRHeadsetProvider::getSupportedSchemeArgs() {
  return {"modelName", "width", "height", "refreshRate", "angles", "position"};
};

const map<string, size_t> VRHeadsetProvider::getScheme() {
  return {{"modelName", 0},
          {"width", 1},
          {"height", 2},
          {"refreshRate", 3},
          {"angles", 4},
          {"position", 5}};
};

string VRHeadsetProvider::get(unique_ptr<AbstractItem> const &target, string const &fieldName) {
  auto i = getters.find(fieldName);

  if (i == getters.end()) {
    throw runtime_error("Unknown field: " + fieldName);
  }

  string field = i->second(static_cast<VRHeadset const &>(*target));

  return field;
}

void VRHeadsetProvider::set(unique_ptr<AbstractItem> &target,
                            string const &fieldName,
                            string const &value) {
  auto i = setters.find(fieldName);

  if (i == setters.end()) {
    throw runtime_error("Unknown field: " + fieldName);
  }

  i->second(static_cast<VRHeadset &>(*target), value);
}

unique_ptr<AbstractItem> VRHeadsetProvider::create() {
  return make_unique<VRHeadset>();
}
