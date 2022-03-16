// some utilify functions

#include <sstream>
#include <string>

namespace Utils {
std::string getLine(std::string fileName, uint32_t lineNumber) {
  std::string line;
  std::ifstream filestream(fileName);
  if (filestream.is_open()) {
    uint32_t i = 1;
    while (std::getline(filestream, line)) {
      if (i++ == lineNumber) {
        break;
      }
    }
  }
  return line;
}

std::string getToken(std::string str, uint32_t tokenNumber) {
  std::istringstream linestream(str);
  std::string token;

  uint32_t i = 1;
  while (linestream >> token) {
    if (i++ == tokenNumber) {
      break;
    }
  }
  return token;
}

std::string getTokenInLine(std::string fileName, uint32_t lineNumber,
                           uint32_t tokenNumber) {
  std::string line = getLine(fileName, lineNumber);
  return getToken(line, tokenNumber);
}
}  // namespace Utils
