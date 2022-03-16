#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return (float) LinuxParser::ActiveJiffies(pid) / (UpTime() * sysconf(_SC_CLK_TCK)); }

// TODO: Return the command that generated this process
string Process::Command() const { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid); }

// TODO: Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.CpuUtilization();
}