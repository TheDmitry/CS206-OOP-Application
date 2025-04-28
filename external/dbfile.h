#pragma once

#include <filesystem>
#include <format>
#include <map>
#include <string>
#include <vector>

#include "providers/abstractprovider.h"

class DbFile {
public:
  DbFile(std::string const &path)
    : currentPath(path) {}
  DbFile()
    : currentPath("") {}

  ~DbFile();

  std::string getCurrentPath() const;
  std::vector<std::string> &getDataBuffer();
  std::vector<std::string> &getInfoBuffer();

  void setCurrentPath(const std::string path);

  void read();
  void read(const std::string &path);

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
      std::stringstream sstream{i};
      std::string part;
      std::vector<std::string> t_parts;
      while (getline(sstream, part, ';')) {
        t_parts.push_back(part);
      }

      const std::vector<std::string> parts{t_parts};
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
      std::vector<std::string> values;
      for (auto &i : provider->getSupportedSchemeArgs()) {
        values.push_back(provider->get(item, i));
      }

      std::string data;
      for (auto &i : values)
        data += format("{};", i);

      dataBuffer.push_back(data);
    }
  }

  // Returns currentScheme
  std::map<std::string, size_t> const &getScheme() const;

  // Returns name of field inside currentScheme with index [key]
  std::string const &getSchemeField(size_t key) const;

  // Allows to get access to current provider. Provider cannot be modified.
  std::shared_ptr<AbstractProvider> const &getProvider() const;

  // Makes by name(X) association to appropriate provider for [provider name=X] operator
  static void registerProvider(std::string const &name, std::shared_ptr<AbstractProvider> provider);
  std::map<std::string, size_t> currentScheme;

private:
  std::string currentPath;
  std::filesystem::file_time_type modified;

  std::vector<std::string> dataBuffer;
  std::vector<std::string> infoBuffer;

  static constexpr std::string SUPPORTED_VERSION = "2";
  static std::map<std::string, std::shared_ptr<AbstractProvider>> providers;

  /* Provider/Scheme */
  std::shared_ptr<AbstractProvider> provider = nullptr;

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
    std::string content;
    std::map<std::string, std::string> args;
  };

  OperatorResult parseOperator(const std::string &line, int lineNumber);
};
