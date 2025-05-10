#pragma once

#include "items/abstractitem.h"
#include <map>
#include <memory>
#include <span>
#include <string>
#include <vector>

class AbstractProvider {
public:
  virtual ~AbstractProvider() = default;
  AbstractProvider() = default;

  virtual const std::string getName() const = 0;
  virtual const std::string getItemName() const = 0;
  virtual const std::string getTrItemName() const = 0;
  virtual const std::vector<std::string> getSupportedSchemeArgs() const = 0;
  virtual const std::map<std::string, size_t> getScheme() const = 0;

  virtual std::string get(std::shared_ptr<AbstractItem> const &target,
                          std::string const &fieldName) const
    = 0;
  virtual void set(std::shared_ptr<AbstractItem> &target,
                   std::string const &fieldName,
                   std::string const &value)
    = 0;

  // returns -1 if less, 0 if equals, 1 if greater
  virtual int compare(std::shared_ptr<AbstractItem> const &a,
                      std::shared_ptr<AbstractItem> const &b,
                      std::string const &fieldName) const
    = 0;

  virtual std::shared_ptr<AbstractItem> create() = 0;

  virtual bool isChartSupported() const = 0;
  virtual const std::string getChartHeaderField() const = 0;
  virtual const std::vector<std::string> getChartFields() const = 0;
};
