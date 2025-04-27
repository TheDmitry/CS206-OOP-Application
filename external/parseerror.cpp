#include "external/parseerror.h"
#include <format>
#include <stdexcept>

ParseError::ParseError(std::string const &message, int lineNum, std::string const &line) noexcept
  : std::runtime_error(message.c_str())
  , throwMessage(message)
  , lineNum(lineNum)
  , line(line) {};

char const *ParseError::what() const noexcept {
  static const std::string message = std::format("ParseError on line {}: \"{}\" -> {}",
                                                 std::to_string(lineNum),
                                                 line,
                                                 throwMessage);
  return message.c_str();
}
