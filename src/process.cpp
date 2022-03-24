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

// Return this process's ID
int Process::Pid() const { return pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
  return (float)LinuxParser::ActiveJiffies(pid) /
         (this->UpTime() * sysconf(_SC_CLK_TCK));
}

// Return the command that generated this process
string Process::Command() const { return LinuxParser::Command(pid); }

// Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid); }

// Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(pid); }

// Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return this->CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(Process const& a) const {
  return this->CpuUtilization() > a.CpuUtilization();
}