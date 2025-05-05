#pragma once

#include <memory>

class AbstractItem {
public:
  virtual ~AbstractItem() = default;
  virtual std::shared_ptr<AbstractItem> clone() const = 0;
};
