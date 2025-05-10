#pragma once

#include "external/vector3.h"
#include "items/abstractitem.h"
#include "items/vrheadset.h"
#include "providers/abstractprovider.h"

#include <cstdlib>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

class VRHeadsetProvider : public AbstractProvider {
public:
  VRHeadsetProvider();
  const std::string getName() const override;
  const std::string getItemName() const override;
  const std::string getTrItemName() const override;
  const std::vector<std::string> getSupportedSchemeArgs() const override;
  const std::map<std::string, size_t> getScheme() const override;

  std::string get(std::shared_ptr<AbstractItem> const &target,
                  std::string const &fieldName) const override;
  void set(std::shared_ptr<AbstractItem> &target,
           std::string const &fieldName,
           std::string const &value) override;

  // returns -1 if less, 0 if equals, 1 if greater
  int compare(std::shared_ptr<AbstractItem> const &a,
              std::shared_ptr<AbstractItem> const &b,
              std::string const &fieldName) const override;

  std::shared_ptr<AbstractItem> create() override;

  bool isChartSupported() const override;
  const std::string getChartHeaderField() const override;
  const std::vector<std::string> getChartFields() const override;

private:
  const std::map<std::string, std::function<std::string(VRHeadset const &)>> getters = {
    {"width",
     [](VRHeadset const &target) -> std::string { return std::to_string(target.getWidth()); }},
    {"height",
     [](VRHeadset const &target) -> std::string { return std::to_string(target.getHeight()); }},
    {"refreshRate",
     [](VRHeadset const &target) -> std::string { return std::to_string(target.getRefreshRate()); }},
    {"angles",
     [](VRHeadset const &target) -> std::string {
       return Vector3::vectorToString(target.getAngles());
     }},
    {"position",
     [](VRHeadset const &target) -> std::string {
       return Vector3::vectorToString(target.getPosition());
     }},
    {"modelName", [](VRHeadset const &target) -> std::string { return target.getModelName(); }},
  };

  const std::map<std::string, std::function<void(VRHeadset &, const std::string &)>> setters = {
    {"width", [](VRHeadset &target, const std::string &val) { target.setWidth(stoi(val)); }},
    {"height", [](VRHeadset &target, const std::string &val) { target.setHeight(stoi(val)); }},
    {"refreshRate",
     [](VRHeadset &target, const std::string &val) { target.setRefreshRate(stof(val)); }},
    {"angles",
     [](VRHeadset &target, const std::string &val) {
       target.setAngles(Vector3::vectorFromString(val));
     }},
    {"position",
     [](VRHeadset &target, const std::string &val) {
       target.setPosition(Vector3::vectorFromString(val));
     }},
    {"modelName", [](VRHeadset &target, const std::string &val) { target.setModelName(val); }},
  };
};
