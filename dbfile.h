#pragma once
#include <filesystem>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "vrheadset.h"

class DbFile
{
public:
  DbFile(std::string path);
  DbFile();
  ~DbFile();

  std::string getCurrentPath() const;
  std::vector<std::string> &getDataBuffer();
  std::vector<std::string> &getInfoBuffer();

  void setCurrentPath();

  void read();
  void read(const string &path);

  void update(std::list<VRHeadset> &list);
  void write();
  void parse(std::list<VRHeadset> &list);

  void clear();

  void reset();

  bool isDataEmpty();
  bool isInfoEmpty();
  bool isBuffersEmpty();

  bool isFileExists();
  bool isFileEmpty();
  bool isFileModified();

private:
  std::string currentPath;
  std::filesystem::file_time_type modified;

  std::vector<std::string> dataBuffer;
  std::vector<std::string> infoBuffer;

  static constexpr std::string SUPPORTED_VERSION = "2";

  enum class OPS {
    DB,
    INFO,
    DATA,
    END,
    CLOSE,
    TEXT,
  };

  struct OperatorResult
  {
    OPS op;
    string content;
    std::map<std::string, std::string> args;
  };

  OperatorResult parseOperator(const std::string &line);
};
