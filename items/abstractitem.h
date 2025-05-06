#pragma once

#include <QObject>
#include <memory>

class AbstractItem {
public:
  virtual ~AbstractItem() = default;
  virtual std::shared_ptr<AbstractItem> clone() const = 0;

  virtual std::string print() const = 0;
  virtual void print(std::ostream &os) const = 0;

  virtual bool operator==(std::shared_ptr<AbstractItem> const &item) const = 0;
  virtual bool operator!=(std::shared_ptr<AbstractItem> const &item) const = 0;
};
