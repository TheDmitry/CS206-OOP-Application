#pragma once
#include <filesystem>
#include <format>
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
  void parse(T &headsets) {
    for (auto &i : dataBuffer) {
      stringstream sstream{i};
      string part;
      vector<string> parts;

      while (getline(sstream, part, ';')) {
        parts.push_back(part);
      }

      VRHeadset vr{
        stoi(parts[currentScheme["width"]]),                         // width
        stoi(parts[currentScheme["height"]]),                        // height
        stof(parts[currentScheme["refreshRate"]]),                   // refreshRate
        Vector3::vectorFromString(parts[currentScheme["angles"]]),   // v3(angles)
        Vector3::vectorFromString(parts[currentScheme["position"]]), // v3(position)
        parts[currentScheme["modelName"]]                            // modelName
      };

      headsets.push_back(vr);
    }
  }

  template<typename T>
  void update(T &headsets) {
    dataBuffer.clear();
    dataBuffer.shrink_to_fit();

    for (auto &headset : headsets) {
      std::vector<std::string> values;
      for (const auto &key : SUPPORTED_SCHEME_ARGS) {
        switch (currentScheme.at(key)) {
          case 0: {
            values.push_back(headset.getModelName());
          } break;
          case 1: {
            values.push_back(to_string(headset.getWidth()));
          } break;
          case 2: {
            values.push_back(to_string(headset.getHeight()));
          } break;
          case 3: {
            values.push_back(to_string(headset.getRefreshRate()));
          } break;
          case 4: {
            values.push_back(Vector3::vectorToString(headset.getAngles()));
          } break;
          case 5: {
            values.push_back(Vector3::vectorToString(headset.getPosition()));
          } break;
          default: {
            throw invalid_argument("Unnable to write file. Invalid scheme");
          } break;
        }
      }

      string data;
      for (auto &i : values)
        data += format("{};", i);

      dataBuffer.push_back(data);
    }
  }

private:
  std::string currentPath;
  std::filesystem::file_time_type modified;

  std::vector<std::string> dataBuffer;
  std::vector<std::string> infoBuffer;

  static constexpr std::string SUPPORTED_VERSION = "2";

  /* SCHEME */
  static const std::vector<std::string> SUPPORTED_SCHEME_ARGS;
  static const std::map<std::string, int> defaultScheme;
  std::map<std::string, int> currentScheme = defaultScheme;

  enum class OPS {
    DB,
    INFO,
    DATA,
    END,
    CLOSE,
    SCHEME,
    TEXT,
  };

  struct OperatorResult {
    OPS op;
    string content;
    std::map<std::string, std::string> args;
  };

  OperatorResult parseOperator(const std::string &line);
};
