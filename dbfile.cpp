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
      isSupported = result.args["version"] == SUPPORTED_VERSION;
      continue;
    }

    if (!isSupported)
      throw invalid_argument("Unsupported db file");

    switch (result.op) {
      case OPS::DB:
        {
          throw invalid_argument(
            "[db version=X] should appear only once in very first line of the file");
        }
        break;

      case OPS::CLOSE:
        {
          shouldEnd = true;
        }
        break;

      case OPS::DATA:
        {
          if (shouldData)
            throw invalid_argument("Invalid db file format. Data-block already opened");

          if (shouldInfo)
            throw invalid_argument(
              "Invalid db file format. It's not allowed to open data-block in info-block");

          shouldData = true;
        }
        break;

      case OPS::INFO:
        {
          if (shouldInfo)
            throw invalid_argument("Invalid db file format. Info-block already opened. It's not "
                                   "allowed to have two or more info-blocks in one file");

          if (shouldData)
            throw invalid_argument(
              "Invalid db file format. Info-block cannot be opened in Data-block");

          shouldInfo = true;
        }
        break;

      case OPS::END:
        {
          if (!shouldData && !shouldInfo)
            throw invalid_argument("Invalid db file format. No blocks were opened");

          if (shouldInfo)
            shouldInfo = false;
          if (shouldData)
            shouldData = false;
        }
        break;

      case OPS::SCHEME:
        {
          size_t schemeMaxSize = SUPPORTED_SCHEME_ARGS.size();

          if (result.args.size() != schemeMaxSize)
            throw invalid_argument(
              "Invalid db file format. Invalid scheme: invalid amount of args");

          currentScheme.clear();

          for (auto &i : result.args) {
            size_t index = stoi(i.second);

            if (index >= schemeMaxSize)
              throw invalid_argument("Invalid db file format. Invalid scheme: index out of bounds");

            if (index < 0)
              throw invalid_argument("Invalid db file format. Invalid scheme: negative index");

            string key = i.first;
            currentScheme[key] = index;
          }

          if (currentScheme.size() != SUPPORTED_SCHEME_ARGS.size())
            throw invalid_argument("Invalid db file format. Invalid scheme: Duplicated keys");
        }
        break;

      case OPS::TEXT:
        {
          if (shouldData)
            dataBuffer.push_back(line);
          if (shouldInfo)
            infoBuffer.push_back(line);
        }
        break;

      default:
        {
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

  string dbVersion = format("[db version={}]", SUPPORTED_VERSION);

  string schemeParts;
  for (auto &i : currentScheme) {
    size_t index = i.second;
    string key = i.first;

    string arg = format(" {}={}", key, index);

    schemeParts += arg;
  }
  string scheme = format("[scheme{}]", schemeParts);

  string content = format("{}\n{}\n[info]{}\n[end]\n[data]{}\n[end]\n[close]",
                          dbVersion,
                          scheme,
                          infoContent,
                          dataContent);

  file << content;
  file.close();
  modified = filesystem::last_write_time(currentPath);
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
  currentScheme = defaultScheme;
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
  else if (tokens[0] == "close")
    result.op = OPS::CLOSE;
  else if (tokens[0] == "scheme")
    result.op = OPS::SCHEME;
  else
    throw invalid_argument("Invalid db file format. Invalid operator " + line);

  for (size_t i = 1; i < tokens.size(); ++i) {
    string arg = tokens[i];

    size_t eq_pos = arg.find('=');

    if (eq_pos != string::npos) {
      string key = arg.substr(0, eq_pos);
      string value = arg.substr(eq_pos + 1);
      result.args[key] = value;
    }
  }

  return result;
}

const vector<string> DbFile::SUPPORTED_SCHEME_ARGS
  = {"modelName", "width", "height", "refreshRate", "angles", "position"};

const std::map<std::string, int> DbFile::defaultScheme = {{"modelName", 0},
                                                          {"width", 1},
                                                          {"height", 2},
                                                          {"refreshRate", 3},
                                                          {"angles", 4},
                                                          {"position", 5}};
