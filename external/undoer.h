#pragma once

#include <algorithm>
#include <list>

template<typename T>
class Undoer {
public:
  Undoer(size_t memorySize = 50)
    : storage{}
    , memorySize(memorySize) {}

  T rewind() {
    if (storage.empty())
      return T{};

    T to;
    T from = storage.back();
    std::copy(from.begin(), from.end(), std::back_inserter(to));
    storage.pop_back();

    return to;
  }

  void remember(T target) {
    if ((storage.size() + 1) > memorySize)
      storage.pop_back();
    storage.push_back(target);
  }

  size_t getMemorySize() { return memorySize; }
  size_t getStoredSize() { return storage.size(); }
  bool isEmpty() { return storage.empty(); }
  void reset() {
    storage.clear();
    storage.resize(0);
  }

private:
  std::list<T> storage;
  size_t memorySize;
};
