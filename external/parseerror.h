#pragma once

#include <stdexcept>
#include <string>

class ParseError : public std::runtime_error {
public:
  ParseError(std::string const &message, int lineNum, std::string const &line) noexcept;
  const char *what() const noexcept override;

private:
  std::string throwMessage;
  int lineNum;
  std::string line;
};
