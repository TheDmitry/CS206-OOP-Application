#include "parseerror.h"
#include <format>

using namespace std;

ParseError::ParseError(string const &message, int lineNum, string const &line) noexcept
  : runtime_error(message.c_str())
  , throwMessage(message)
  , lineNum(lineNum)
  , line(line) {};

char const *ParseError::what() const noexcept {
  static const string message = format("ParseError on line {}: \"{}\" -> {}",
                                       to_string(lineNum),
                                       line,
                                       throwMessage);
  return message.c_str();
}
