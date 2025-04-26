#pragma once

#include "external/vector3.h"
#include "items/abstractitem.h"
#include "items/vrheadset.h"
#include "providers/abstractprovider.h"

#include <cstdlib>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class VRHeadsetProvider : public AbstractProvider {
public:
  VRHeadsetProvider();
  const string getName() override;
  const vector<string> getSupportedSchemeArgs() override;
  const map<string, size_t> getScheme() override;

  string get(unique_ptr<AbstractItem> const &target, string const &fieldName) override;
  void set(unique_ptr<AbstractItem> &target, string const &fieldName, string const &value) override;
  unique_ptr<AbstractItem> create() override;

private:
  /*
   * width -> int 
   * height -> int
   * 
   * 
   */

  const map<string, function<string(VRHeadset const &)>> getters = {
    {"width", [](VRHeadset const &target) -> string { return to_string(target.getWidth()); }},
    {"height", [](VRHeadset const &target) -> string { return to_string(target.getHeight()); }},
    {"refreshRate",
     [](VRHeadset const &target) -> string { return to_string(target.getRefreshRate()); }},
    {"angles",
     [](VRHeadset const &target) -> string { return Vector3::vectorToString(target.getAngles()); }},
    {"position",
     [](VRHeadset const &target) -> string {
       return Vector3::vectorToString(target.getPosition());
     }},
    {"modelName", [](VRHeadset const &target) -> string { return target.getModelName(); }},
  };

  const map<string, function<void(VRHeadset &, const string &)>> setters = {
    {"width", [](VRHeadset &target, const string &val) { target.setWidth(stoi(val)); }},
    {"height", [](VRHeadset &target, const string &val) { target.setHeight(stoi(val)); }},
    {"refreshRate", [](VRHeadset &target, const string &val) { target.setRefreshRate(stof(val)); }},
    {"angles",
     [](VRHeadset &target, const string &val) { target.setAngles(Vector3::vectorFromString(val)); }},
    {"position",
     [](VRHeadset &target, const string &val) {
       target.setPosition(Vector3::vectorFromString(val));
     }},
    {"modelName", [](VRHeadset &target, const string &val) { target.setModelName(val); }},
  };
};
