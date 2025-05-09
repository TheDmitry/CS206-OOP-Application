#include <format>

#include "external/dberror.h"

#include <QObject>

using namespace std;

DbError::DbError(string const &message, string const &fileName) noexcept
  : runtime_error(message.c_str())
  , throwMessage(message)
  , fileName(fileName) {};

char const *DbError::what() const noexcept {
  static const string message = format("DbError {} {} -> {}",
                                       QObject::tr("in").toStdString(),
                                       fileName,
                                       throwMessage);
  return message.c_str();
}
