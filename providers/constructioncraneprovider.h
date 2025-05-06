#pragma once

#include "abstractprovider.h"
#include "items/constructioncrane.h"

class ConstructionCraneProvider : public AbstractProvider {
public:
  ConstructionCraneProvider();

  const std::string getName() override;
  const std::vector<std::string> getSupportedSchemeArgs() override;
  const std::map<std::string, size_t> getScheme() override;

  std::string get(std::shared_ptr<AbstractItem> const &target,
                  std::string const &fieldName) const override;
  void set(std::shared_ptr<AbstractItem> &target,
           std::string const &fieldName,
           std::string const &value) override;

  int compare(std::shared_ptr<AbstractItem> const &a,
              std::shared_ptr<AbstractItem> const &b,
              std::string const &fieldName) const override;

  std::shared_ptr<AbstractItem> create() override;

private:
  const std::map<std::string, std::function<std::string(ConstructionCrane const &)>> getters = {
    {"weight",
     [](ConstructionCrane const &target) -> std::string {
       return std::to_string(target.getWeight());
     }},
    {"height",
     [](ConstructionCrane const &target) -> std::string {
       return std::to_string(target.getHeight());
     }},
    {"angles",
     [](ConstructionCrane const &target) -> std::string {
       return Vector3::vectorToString(target.getAngles());
     }},
    {"position",
     [](ConstructionCrane const &target) -> std::string {
       return Vector3::vectorToString(target.getPosition());
     }},
    {"serial", [](ConstructionCrane const &target) -> std::string { return target.getSerial(); }},
    {"modelName",
     [](ConstructionCrane const &target) -> std::string { return target.getModelName(); }},
  };

  const std::map<std::string, std::function<void(ConstructionCrane &, const std::string &)>> setters
    = {
      {"weight",
       [](ConstructionCrane &target, const std::string &val) { target.setWeight(stof(val)); }},
      {"height",
       [](ConstructionCrane &target, const std::string &val) { target.setHeight(stof(val)); }},
      {"angles",
       [](ConstructionCrane &target, const std::string &val) {
         target.setAngles(Vector3::vectorFromString(val));
       }},
      {"position",
       [](ConstructionCrane &target, const std::string &val) {
         target.setPosition(Vector3::vectorFromString(val));
       }},
      {"serial", [](ConstructionCrane &target, const std::string &val) { target.setSerial(val); }},
      {"modelName",
       [](ConstructionCrane &target, const std::string &val) { target.setModelName(val); }},
    };
};
