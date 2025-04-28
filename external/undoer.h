#pragma once

#include <algorithm>
#include <iostream>
#include <list>

// TODO: allow only unique_ptrs ?
template<typename T>
class Undoer {
public:
  Undoer(size_t memorySize = 50)
    : storage{}
    , memorySize(memorySize) {}

  T rewind() {
    std::cout << "rewind call p1 -> " << storage.size() << std::endl;
    if (storage.empty())
      return T{};

    std::cout << "rewind call p2 -> " << storage.size() << std::endl;

    T to;
    T from = storage.back();
    std::copy(from.begin(), from.end(), std::back_inserter(to));
    storage.pop_back();

    return to;
    std::cout << "rewind call p3 -> " << storage.size() << std::endl;
  }

  void remember(T target) {
    if ((storage.size() + 1) > memorySize)
      storage.pop_back();
    std::cout << "remember call p1 -> " << storage.size() << std::endl;
    storage.push_back(target);
    std::cout << "remember call p2 -> " << storage.size() << std::endl;
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
