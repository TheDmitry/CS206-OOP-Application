#include "external/dberror.h"
#include <format>
#include <stdexcept>

DbError::DbError(std::string const &message, std::string const &fileName) noexcept
  : std::runtime_error(message.c_str())
  , throwMessage(message)
  , fileName(fileName) {};

char const *DbError::what() const noexcept {
  static const std::string message = std::format("DbError in {} -> {}", fileName, throwMessage);
  return message.c_str();
}
