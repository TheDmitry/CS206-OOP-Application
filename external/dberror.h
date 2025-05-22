#pragma once

#include <stdexcept>
#include <string>

class DbError : public std::runtime_error {
public:
  DbError(std::string const &message,
          std::string const &fileName = "") noexcept;
  const char *what() const noexcept override;

private:
  std::string throwMessage;
  std::string fileName;
};
