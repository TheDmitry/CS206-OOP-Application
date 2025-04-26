#pragma once
#include <filesystem>
#include <format>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "providers/abstractprovider.h"

using namespace std;

class DbFile {
public:
  DbFile(string const &path)
    : currentPath(path) {}
  DbFile()
    : currentPath("") {}

  ~DbFile();

  string getCurrentPath() const;
  vector<string> &getDataBuffer();
  vector<string> &getInfoBuffer();

  void setCurrentPath(const string path);

  void read();
  void read(const string &path);

  void write();

  void clear();

  void reset();

  bool isDataEmpty();
  bool isInfoEmpty();
  bool isBuffersEmpty();

  bool isFileExists();
  bool isFileEmpty();
  bool isFileModified();

  template<typename T>
  void parse(T &items) {
    for (auto &i : dataBuffer) {
      stringstream sstream{i};
      string part;
      vector<string> t_parts;
      while (getline(sstream, part, ';')) {
        t_parts.push_back(part);
      }

      const vector<string> parts{t_parts};
      auto item = provider->create();

      for (const auto &it : provider->getSupportedSchemeArgs()) {
        provider->set(item, it, t_parts[currentScheme[it]]);
      }

      items.push_back(std::move(item));
    }
  }

  template<typename T>
    requires requires(typename T::value_type v) {
      { v.get() } -> std::convertible_to<AbstractItem *>;
    }
  void update(T &items) {
    dataBuffer.clear();
    dataBuffer.shrink_to_fit();

    for (auto &item : items) {
      vector<string> values;
      for (auto &i : provider->getSupportedSchemeArgs()) {
        values.push_back(provider->get(item, i));
      }

      string data;
      for (auto &i : values)
        data += format("{};", i);

      dataBuffer.push_back(data);
    }
  }

  // Returns currentScheme
  map<string, size_t> const &getScheme() const;

  // Returns name of field inside currentScheme with index [key]
  string const &getSchemeField(size_t key) const;

  // Allows to get access to current provider. Provider cannot be modified.
  shared_ptr<AbstractProvider> const &getProvider() const;

  // Makes by name(X) association to appropriate provider for [provider name=X] operator
  static void registerProvider(string const &name, shared_ptr<AbstractProvider> provider);
  map<string, size_t> currentScheme;

private:
  string currentPath;
  filesystem::file_time_type modified;

  vector<string> dataBuffer;
  vector<string> infoBuffer;

  static constexpr string SUPPORTED_VERSION = "2";
  static map<string, shared_ptr<AbstractProvider>> providers;

  /* Provider/Scheme */
  shared_ptr<AbstractProvider> provider = nullptr;

  enum class OPS {
    DB,
    INFO,
    DATA,
    END,
    CLOSE,
    SCHEME,
    PROVIDER,
    TEXT,
  };

  struct OperatorResult {
    OPS op;
    string content;
    map<string, string> args;
  };

  OperatorResult parseOperator(const string &line);
};
