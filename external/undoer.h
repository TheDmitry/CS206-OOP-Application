#pragma once

#include "qdebug.h"
#include "qlogging.h"
#include <algorithm>
#include <functional>
#include <list>
#include <stdexcept>
#include <unordered_map>

template<typename T>
class Undoer {
public:
  using ItemType = typename T::value_type::element_type;
  using Patch = std::unordered_map<size_t, std::shared_ptr<ItemType>>;

  Undoer(const size_t memorySize = 50)
    : memorySize(memorySize) {}

  T rewind() {
    if (storage.empty())
      return T{};

    const auto &[base_state, reverse_patch] = storage.back();

    T restored_state = deepCopy(base_state);
    for (const auto &[index, item] : reverse_patch) {
      if (index < restored_state.size()) {
        restored_state[index] = item ? item->clone() : nullptr;
      }
    }

    storage.pop_back();
    return restored_state;
  }

  void remember(const T &target) {
    if (storage.size() >= memorySize)
      storage.pop_front();

    Patch changes;
    if (!storage.empty()) {
      auto const &last_state = storage.back().first;
      for (size_t i = 0; i < target.size(); ++i) {
        if (i >= last_state.size() || !itemsEqual(last_state[i], target[i])) {
          changes[i] = target[i] ? target[i]->clone() : nullptr;
        }
      }
    } else {
      for (size_t i = 0; i < target.size(); ++i) {
        changes[i] = target[i] ? target[i]->clone() : nullptr;
      }
    }

    storage.emplace_back(deepCopy(target), std::move(changes));
  }

  bool remember(const T &target, const std::function<void()> &callback) {
    remember(target);
    try {
      callback();
      return true;
    } catch (const std::runtime_error &e) {
      qDebug() << "Error on remember:" << e.what();
      storage.pop_back();
      return false;
    }
  }

  size_t getMemorySize() const { return memorySize; }
  size_t getStoredSize() const { return storage.size(); }
  bool isEmpty() const { return storage.empty(); }
  void reset() { storage.clear(); }

private:
  std::list<std::pair<T, Patch>> storage;
  size_t memorySize;

  T deepCopy(const T &source) const {
    T copy;
    copy.reserve(source.size());
    for (const auto &item : source) {
      copy.push_back(item ? item->clone() : nullptr);
    }
    return copy;
  }

  bool itemsEqual(const std::shared_ptr<ItemType> &a, const std::shared_ptr<ItemType> &b) const {
    if (!a && !b)
      return true;
    if (!a || !b)
      return false;
    return a == b;
  }
};
