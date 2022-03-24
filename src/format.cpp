#include "format.h"

#include <cstdio>
#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  long hours = seconds / (3600);  // 60 * 60 = 3600
  seconds %= (3600);
  long minutes = seconds / 60;
  seconds %= 60;

  // size includes the string terminating character
  char formatted[9];
  sprintf(formatted, "%02ld:%02ld:%02ld", hours, minutes, seconds);
  return std::string(formatted);
}