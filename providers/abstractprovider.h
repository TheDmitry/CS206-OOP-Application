#pragma once

#include "items/abstractitem.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

using namespace std;

class AbstractProvider {
public:
  virtual ~AbstractProvider() = default;
  AbstractProvider() = default;

  virtual const string getName() = 0;
  virtual const vector<string> getSupportedSchemeArgs() = 0;
  virtual const map<string, size_t> getScheme() = 0;

  virtual string get(shared_ptr<AbstractItem> const &target, string const &fieldName) const = 0;
  virtual void set(shared_ptr<AbstractItem> &target, string const &fieldName, string const &value)
    = 0;

  // returns -1 if less, 0 if equals, 1 if greater
  virtual int compare(shared_ptr<AbstractItem> const &a,
                      shared_ptr<AbstractItem> const &b,
                      string const &fieldName) const
    = 0;

  virtual shared_ptr<AbstractItem> create() = 0;

protected:
};
