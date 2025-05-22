#include <fstream>

#include "dbfile.h"
#include "external/dberror.h"
#include "external/parseerror.h"
#include "qdebug.h"
#include "qlogging.h"

#include <QObject>

using namespace std;

DbFile::~DbFile() { reset(); }

string DbFile::getCurrentPath() const { return currentPath; }
vector<string> &DbFile::getDataBuffer() { return dataBuffer; }
vector<string> &DbFile::getInfoBuffer() { return infoBuffer; }

void DbFile::setCurrentPath(const string path) { currentPath = path; }

void DbFile::read() {
  // assume it's bug
  if (!isFileExists())
    throw DbError(
        QObject::tr("Error #1. It's ambitious to read non existent file")
            .toStdString(),
        currentPath);

  if (isFileEmpty())
    throw DbError(QObject::tr("Error #2. File is empty").toStdString(),
                  currentPath);

  if (!dataBuffer.empty())
    clear();
  if (!infoBuffer.empty())
    clear();

  ifstream file{currentPath};

  if (!file)
    throw DbError(QObject::tr("Error #3. Unable to open file").toStdString(),
                  currentPath);

  string line;

  bool shouldData = false;
  bool shouldInfo = false;
  bool shouldEnd = false;
  bool isSupported = false;
  int lineNumber = 0;
  while (getline(file, line)) {
    if (shouldEnd)
      break;

    lineNumber++;

    if (line.empty())
      continue;

    OperatorResult result = parseOperator(line, lineNumber);

    if (!isSupported && result.op == OPS::DB) {
      isSupported = result.args["version"] == SUPPORTED_VERSION;
      continue;
    }

    if (!isSupported)
      throw ParseError(
          QObject::tr("Error #2. Unsuppported db file").toStdString(),
          lineNumber, result.content);

    switch (result.op) {
    case OPS::DB: {
      throw ParseError(QObject::tr("Error #3. [db version=X] should appear "
                                   "only once in very first line of the file")
                           .toStdString(),
                       lineNumber, result.content);
    } break;

    case OPS::CLOSE: {
      shouldEnd = true;
    } break;

    case OPS::DATA: {
      if (shouldData)
        throw ParseError(
            QObject::tr("Error #4. Data-block already opened").toStdString(),
            lineNumber, result.content);

      if (shouldInfo)
        throw ParseError(
            QObject::tr(
                "Error #5. It's not allowed to open data-block in info-block")
                .toStdString(),
            lineNumber, result.content);

      shouldData = true;
    } break;

    case OPS::INFO: {
      if (shouldInfo)
        throw ParseError(
            QObject::tr(
                "Error #6. Info-block already opened. "
                "It's not allowed to have two or more info-blocks in one file")
                .toStdString(),
            lineNumber, result.content);

      if (shouldData)
        throw ParseError(
            QObject::tr(
                "Error #7. Info-block cannot be opened inside Data-block")
                .toStdString(),
            lineNumber, result.content);

      shouldInfo = true;
    } break;

    case OPS::END: {
      if (!shouldData && !shouldInfo)
        throw ParseError(
            QObject::tr("Error #8. No blocks were opened").toStdString(),
            lineNumber, result.content);

      if (shouldInfo)
        shouldInfo = false;
      if (shouldData)
        shouldData = false;
    } break;

    case OPS::SCHEME: {
      size_t schemeMaxSize = provider->getSupportedSchemeArgs().size();

      if (result.args.size() != schemeMaxSize)
        throw ParseError(
            QObject::tr("Error #9. Invalid scheme: invalid amount of args")
                .toStdString(),
            lineNumber, result.content);

      currentScheme.clear();

      for (auto &i : result.args) {
        size_t index = stoi(i.second);

        if (index >= schemeMaxSize)
          throw ParseError(
              QObject::tr("Error #10. Invalid scheme: index out of bounds")
                  .toStdString(),
              lineNumber, result.content);

        if (index < 0)
          throw ParseError(
              QObject::tr("Error #11. Invalid scheme: negative index")
                  .toStdString(),
              lineNumber, result.content);

        string key = i.first;
        currentScheme[key] = index;
      }

      if (currentScheme.size() != provider->getSupportedSchemeArgs().size())
        throw ParseError(
            QObject::tr("Error #12. Invalid scheme: Duplicated keys")
                .toStdString(),
            lineNumber, result.content);
    } break;

    case OPS::PROVIDER: {
      if (result.args.size() < 0 || result.args.size() > 2)
        throw ParseError(
            QObject::tr(
                "Error #13. Operator provider should have only 2 arguments")
                .toStdString(),
            lineNumber, result.content);

      string providerName = result.args["name"];
      if (!DbFile::providers.contains(providerName))
        throw ParseError(
            QObject::tr("Error #14. No such provider registred").toStdString(),
            lineNumber, result.content);

      bool shouldOverwrite = !result.args.contains("noOverwrite");

      provider = DbFile::providers[providerName];
      if (shouldOverwrite)
        currentScheme = provider->getScheme();

    } break;

    case OPS::TEXT: {
      if (shouldData)
        dataBuffer.push_back(line);
      if (shouldInfo)
        infoBuffer.push_back(line);
    } break;

    default: {
      throw ParseError(
          QObject::tr("Error #15. Unsupported operator").toStdString(),
          lineNumber, result.content);
    }
    }
  }

  file.close();
  modified = filesystem::last_write_time(currentPath);
}

void DbFile::read(const string &path) {
  currentPath = path;
  read();
}

void DbFile::write() {
  if (!isFileExists())
    throw DbError(
        QObject::tr("Error #4. It's ambitious to write to non existent file")
            .toStdString(),
        currentPath);

  ofstream file{currentPath};

  if (!file)
    throw DbError(QObject::tr("Error #3. Unable to open file").toStdString(),
                  currentPath);

  string infoContent;
  string dataContent;

  if (infoBuffer.size() > 0) {
    for (auto ib : infoBuffer)
      infoContent.append(format("\n{}", ib));
  }

  if (dataBuffer.size() > 0) {
    for (auto db : dataBuffer)
      dataContent.append(format("\n{}", db));
  }

  string dbVersion = format("[db version={}]\n", SUPPORTED_VERSION);
  string providerTag = format("[provider name={}]\n", provider->getName());
  string schemeParts;
  for (auto &i : currentScheme) {
    size_t index = i.second;
    string key = i.first;

    string arg = format(" {}={}", key, index);

    schemeParts += arg;
  }
  string scheme = format("[scheme{}]\n", schemeParts);

  string content =
      format("{}{}{}[info]{}\n[end]\n[data]{}\n[end]\n[close]", dbVersion,
             providerTag, scheme, infoContent, dataContent);

  file << content;
  file.close();
  modified = filesystem::last_write_time(currentPath);
}

void DbFile::write(std::string const &path) {
  currentPath = path;
  write();
}

void DbFile::create(std::string const &path, std::string const &providerName) {
  if (!providers.contains(providerName))
    throw DbError("Error #5. Unnable to create file, invalid provider provided",
                  path);

  provider = DbFile::providers[providerName];

  write(path);
  read(path);
}

void DbFile::clear() {
  dataBuffer.clear();
  dataBuffer.shrink_to_fit();
  infoBuffer.clear();
  infoBuffer.shrink_to_fit();

  modified = {};
  currentScheme = {};
  provider = nullptr;
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

DbFile::OperatorResult DbFile::parseOperator(string const &line,
                                             int lineNumber) {
  OperatorResult result = {};
  result.op = OPS::TEXT;
  result.content = line;

  if (!line.starts_with('[') || !line.ends_with(']')) {
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
  else if (tokens[0] == "provider")
    result.op = OPS::PROVIDER;
  else
    throw ParseError(
        QObject::tr("Error #1. Invalid operator provided").toStdString(),
        lineNumber, line);

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

// Returns currentScheme
map<string, size_t> const &DbFile::getScheme() const { return currentScheme; };
void DbFile::setScheme(std::map<std::string, size_t> const &scheme) {
  for (auto const &i : scheme) {
    if (!currentScheme.contains(i.first))
      continue;
    currentScheme[i.first] = i.second;
  }
}

// Returns name of all fields inside currentScheme
string const &DbFile::getSchemeField(size_t key) const {
  for (auto const &i : currentScheme) {
    if (i.second == key)
      return i.first;
  }

  throw DbError(
      "Error #7. Out of bounds on getSchemeField(size_t) on index = " +
      to_string(key));
}

DbFile::ProviderType const &DbFile::getProvider() const { return provider; };
void DbFile::setProvider(std::string const &name) {
  if (!providers.contains(name))
    throw DbError("Error #6. Unnable to set provider " + name +
                  ". Provider does not exists");

  provider = providers[name];
  currentScheme = provider->getScheme();
}

/* Static */
map<string, DbFile::ProviderType> DbFile::providers = {};

void DbFile::registerProvider(string const &name,
                              DbFile::ProviderType provider) {
  qDebug() << "Provider\t" << name << "\tregistered";
  DbFile::providers[name] = provider;
}

DbFile::ProviderStorage const &DbFile::getProviders() {
  return DbFile::providers;
}
