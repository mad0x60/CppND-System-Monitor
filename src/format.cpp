#include "format.h"

#include <string>
#include <cstdio>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long hours = seconds / (60 * 60);
    seconds %= (60 * 60);
    long minutes = seconds / 60;
    seconds %= 60;

    // size inclued the string terminating character
    char formatted[9];
    sprintf(formatted, "%02ld:%02ld:%02ld", hours, minutes, seconds);
    return std::string(formatted);
}