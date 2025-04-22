#include "dbfile.h"
#include <filesystem>
#include <format>
#include <fstream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

DbFile::DbFile(string path) : currentPath(path) {}
DbFile::DbFile() : currentPath("") {}
DbFile::~DbFile() { reset(); }

string DbFile::getCurrentPath() const { return currentPath; }
vector<string> &DbFile::getDataBuffer() { return dataBuffer; }
vector<string> &DbFile::getInfoBuffer() { return infoBuffer; }

void DbFile::read() {
  // assume it's bug
  if (!isFileExists())
    throw invalid_argument("It's ambitious to read non existent file");

  if (isFileEmpty())
    throw invalid_argument("File is empty");

  if (!dataBuffer.empty())
    clear();
  if (!infoBuffer.empty())
    clear();

  ifstream file{currentPath};

  if (!file)
    throw invalid_argument("Unnable to open file");

  string line;

  bool shouldData = false;
  bool shouldInfo = false;
  bool shouldEnd = false;
  bool isSupported = false;

  while (getline(file, line)) {
    if (shouldEnd)
      break;

    if (line.empty())
      continue;

    OperatorResult result = parseOperator(line);

    if (!isSupported && result.op == OPS::DB) {
      isSupported = result.args["value"] == SUPPORTED_VERSION;
      continue;
    }

    if (!isSupported)
      throw invalid_argument("Unsupported db file");

    switch (result.op) {
    case OPS::DB: {
      throw invalid_argument(
        "[db version=X] should appear only once in very first line of the file");
    } break;

    case OPS::CLOSE: {
      shouldEnd = true;
    } break;

    case OPS::DATA: {
      if (shouldData)
        throw invalid_argument("Invalid db file format. Data-block already opened");

      if (shouldInfo)
        throw invalid_argument(
          "Invalid db file format. It's not allowed to open data-block in info-block");

      shouldData = true;
    } break;

    case OPS::INFO: {
      if (shouldInfo)
        throw invalid_argument("Invalid db file format. Info-block already opened. It's not "
                               "allowed to have two or more info-blocks in one file");

      if (shouldData)
        throw invalid_argument("Invalid db file format. Info-block cannot be opened in Data-block");

      shouldInfo = true;
    } break;

    case OPS::END: {
      if (!shouldData && !shouldInfo)
        throw invalid_argument("Invalid db file format. No blocks were opened");

      if (shouldInfo)
        shouldInfo = false;
      if (shouldData)
        shouldData = false;
    } break;

    case OPS::TEXT: {
      if (shouldData)
        dataBuffer.push_back(line);
      if (shouldInfo)
        infoBuffer.push_back(line);

    } break;

    default: {
      throw invalid_argument("Invalid db file format. Unsupported operator");
    }
    }
  }

  file.close();
  modified = filesystem::last_write_time(currentPath);
}

void DbFile::read(const string &path)
{
  currentPath = path;
  read();
}

void DbFile::update(list<VRHeadset> &headsets)
{
  dataBuffer.clear();
  dataBuffer.shrink_to_fit();

  for (auto &i : headsets) {
    string data = format("{};{};{};{};{};{}",
                         i.getModelName(),
                         i.getWidth(),
                         i.getHeight(),
                         i.getRefreshRate(),
                         Vector3::vectorToString(i.getAngles()),
                         Vector3::vectorToString(i.getPosition()));
    dataBuffer.push_back(data);
  }
}

void DbFile::write() {
  if (!isFileExists())
    throw invalid_argument("It's ambitious to write to file without path to file");

  ofstream file{currentPath};

  if (!file) {
    throw invalid_argument("Unnable to open file!");
  }

  string infoContent;
  string dataContent;

  if (infoBuffer.size() > 0) {
    for (auto ib : infoBuffer)
      infoContent.append(format("\n{}", ib));
  }

  if (dataBuffer.size() > 0) {
    for (auto ib : dataBuffer)
      dataContent.append(format("\n{}", ib));
  }

  string content = format("[db version=%s][info]{}\n[end]\n[data]{}\n[end]\n[close]",
                          SUPPORTED_VERSION,
                          infoContent,
                          dataContent);

  file << content;
  file.close();
  modified = filesystem::last_write_time(currentPath);
}

void DbFile::parse(list<VRHeadset> &headsets)
{
  for (auto &i : dataBuffer) {
    stringstream sstream{i};
    string part;
    vector<string> parts;

    while (getline(sstream, part, ';')) {
      parts.push_back(part);
    }

    VRHeadset vr{
      stoi(parts[1]),                      // width
      stoi(parts[2]),                      // heightt
      stof(parts[3]),                      // refreshRate
      Vector3::vectorFromString(parts[4]), // v3(angles)
      Vector3::vectorFromString(parts[5]), // v3(position)
      parts[0]                             // modelName
    };

    headsets.push_back(vr);
  }

  VRHeadset empty{};
  headsets.push_back(empty);
}

void DbFile::clear() {
  dataBuffer.clear();
  dataBuffer.shrink_to_fit();
  infoBuffer.clear();
  infoBuffer.shrink_to_fit();

  modified = {};
}

void DbFile::reset() {
  currentPath = "";
  clear();
}

bool DbFile::isDataEmpty() { return dataBuffer.empty(); }
bool DbFile::isInfoEmpty() { return infoBuffer.empty(); }
bool DbFile::isBuffersEmpty() { return isDataEmpty() && isInfoEmpty(); }

bool DbFile::isFileExists() { return filesystem::exists(currentPath); }
bool DbFile::isFileEmpty() {
  if (!isFileExists())
    return false;

  return filesystem::is_empty(currentPath);
}
bool DbFile::isFileModified() {
  if (!isFileExists())
    return false;

  return modified != filesystem::last_write_time(currentPath);
}

DbFile::OperatorResult DbFile::parseOperator(string const &line)
{
  OperatorResult result = {};
  result.op = OPS::TEXT;
  result.content = "";

  if (!line.starts_with('[') || !line.ends_with(']')) {
    result.content = line;
    return result;
  }

  string sanitazed = line.substr(1, line.size() - 2);

  stringstream sanitazedStream{sanitazed};
  vector<string> tokens = {};
  string token;

  while (sanitazedStream >> token) {
    tokens.push_back(token);
  }

  if (tokens.empty()) {
    return result;
  }

  if (tokens[0] == "data")
    result.op = OPS::DATA;
  else if (tokens[0] == "info")
    result.op = OPS::INFO;
  else if (tokens[0] == "end")
    result.op = OPS::END;
  else if (tokens[0] == "db")
    result.op = OPS::DB;

  for (size_t i = 1; i < tokens.size(); ++i) {
    string arg = tokens[i];

    size_t eq_pos = arg.find('=');

    if (eq_pos != std::string::npos) {
      string key = arg.substr(0, eq_pos);
      string value = arg.substr(eq_pos + 1);
      result.args[key] = value;
    }
  }

  return result;
}
