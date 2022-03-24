#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cstring>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "utils.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;

  for (const auto& file : std::filesystem::directory_iterator(kProcDirectory)) {
    string dirPath = string(file.path());
    string dirName =
        dirPath.substr(dirPath.find_last_of("/") + 1, dirPath.length());

    // filter only directories with a name that has digits only.
    if (dirName.find_first_not_of("0123456789") == string::npos) {
      pids.push_back(std::stoi(dirName));
    }
  }
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  long memTotal =
      std::stol(Utils::getTokenInLine(kProcDirectory + kMeminfoFilename, 1, 2));
  long memFree =
      std::stol(Utils::getTokenInLine(kProcDirectory + kMeminfoFilename, 2, 2));
  return (float)(memTotal - memFree) / memTotal;
}

// Read and return the system uptime in seconds
long LinuxParser::UpTime() {
  return std::stol(
      Utils::getTokenInLine(kProcDirectory + kUptimeFilename, 1, 1));
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long totalJiffies = 0;
  for (string stat : CpuUtilization()) {
    totalJiffies += stol(stat);
  }

  return totalJiffies;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return Jiffies() - IdleJiffies(); }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> CpuStats = CpuUtilization();
  long idleTime = stol(CpuStats[CPUStates::kIdle_]);
  long ioWaitTime = stol(CpuStats[CPUStates::kIOwait_]);

  return idleTime + ioWaitTime;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuStats;
  string cpuStatsStr = Utils::getLine(kProcDirectory + kStatFilename, 1);
  std::istringstream cpuStatsStream(cpuStatsStr);
  cpuStatsStream.ignore(5, ' ');  // ignoring the cpu token
  for (string stat; cpuStatsStream >> stat; cpuStats.push_back(stat))
    ;
  return cpuStats;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return std::stol(Utils::getTokenInLine(kProcDirectory + kStatFilename, 9, 2));
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return std::stol(
      Utils::getTokenInLine(kProcDirectory + kStatFilename, 10, 2));
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  vector<string> procStats = procStatTokens(pid);
  if (procStats.empty()) {
    return 0;
  }
  long userTime = stol(procStats[ProcStates::utime_]);
  long kernelTime = stol(procStats[ProcStates::stime_]);
  long childUserTime = stol(procStats[ProcStates::cutime_]);
  long childKernelTime = stol(procStats[ProcStates::cstime_]);

  return userTime + kernelTime + childUserTime + childKernelTime;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string procCmdFile = kProcDirectory + std::to_string(pid) + kCmdlineFilename;
  return Utils::getLine(procCmdFile, 1);
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string procStatusFile =
      kProcDirectory + std::to_string(pid) + kStatusFilename;

  // divide by 1000 to convert from KB to MB
  long vmSizeMB;
  try {
    vmSizeMB = std::stol(Utils::getTokenInLine(procStatusFile, 18, 2)) / 1000;
  } catch (...) {
    vmSizeMB = 0;
  }
  return std::to_string(vmSizeMB);
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::string procStatusFile =
      kProcDirectory + std::to_string(pid) + kStatusFilename;
  return Utils::getTokenInLine(procStatusFile, 9, 2);
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);  // string of UID

  string line;
  std::vector<string> tokens;
  std::ifstream filestream(kPasswordPath);  // /etc/passwd file
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // tokenizing
      for (auto token = strtok(&line[0], ":"); token != NULL;
           token = strtok(NULL, ":")) {
        if (uid.compare(token) == 0) {
          return tokens[0];
        } else {
          tokens.push_back(token);
        }
      }
      tokens.clear();
    }
  }
  return string();
}

// Read and return the uptime of a process in seconds
long LinuxParser::UpTime(int pid) {
  std::string procStatFile =
      kProcDirectory + std::to_string(pid) + kStatFilename;
  vector<string> procStats = procStatTokens(pid);

  long startTime;

  if (procStats.empty()) {
    startTime = 0;
  } else {
    // start time of the process in jiffies
    startTime = stol(procStats[ProcStates::starttime_]);
  }

  // convert jiffies to seconds and subtract if from the total system uptime
  return UpTime() - startTime / sysconf(_SC_CLK_TCK);
}

vector<string> LinuxParser::procStatTokens(int pid) {
  vector<string> procStats;
  string procStatFile = kProcDirectory + std::to_string(pid) + kStatFilename;
  string procStatsStr = Utils::getLine(procStatFile, 1);
  std::istringstream procStatsStream(procStatsStr);
  for (string stat; procStatsStream >> stat; procStats.push_back(stat))
    ;
  return procStats;
}