#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cstring>
#include <sstream>
#include <string>
#include <vector>

#include "utils.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
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

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  long memTotal =
      std::stol(Utils::getTokenInLine(kProcDirectory + kMeminfoFilename, 1, 2));
  long memFree =
      std::stol(Utils::getTokenInLine(kProcDirectory + kMeminfoFilename, 2, 2));
  return (float)(memTotal - memFree) / memTotal;
}

// TODO: Read and return the system uptime in seconds
long LinuxParser::UpTime() {
  return std::stol(
      Utils::getTokenInLine(kProcDirectory + kUptimeFilename, 1, 1));
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long totalJiffies = 0;
  for (string stat : CpuUtilization()) {
    totalJiffies += stol(stat);
  }

  return totalJiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return Jiffies() - IdleJiffies(); }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> CpuStats = CpuUtilization();
  long idleTime = stol(CpuStats[CPUStates::kIdle_]);
  long ioWaitTime = stol(CpuStats[CPUStates::kIOwait_]);

  return idleTime + ioWaitTime;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuStats;
  string cpuStatsStr = Utils::getLine(kProcDirectory + kStatFilename, 1);
  std::istringstream cpuStatsStream(cpuStatsStr);
  cpuStatsStream.ignore(5, ' ');  // ignoring the cpu token
  for (string stat; cpuStatsStream >> stat; cpuStats.push_back(stat))
    ;
  return cpuStats;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return std::stol(Utils::getTokenInLine(kProcDirectory + kStatFilename, 9, 2));
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return std::stol(
      Utils::getTokenInLine(kProcDirectory + kStatFilename, 10, 2));
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  vector<string> procStats = procStatTokens(pid);
  long userTime = stol(procStats[ProcStates::utime_]);
  long kernelTime = stol(procStats[ProcStates::stime_]);
  long childUserTime = stol(procStats[ProcStates::cutime_]);
  long childKernelTime = stol(procStats[ProcStates::cstime_]);

  return userTime + kernelTime + childUserTime + childKernelTime;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string procCmdFile = kProcDirectory + std::to_string(pid) + kCmdlineFilename;
  return Utils::getLine(procCmdFile, 1);
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string procStatusFile =
      kProcDirectory + std::to_string(pid) + kStatusFilename;

  // divide by 1000 to convert from KB to MB
  long vmSizeMB =
      std::stol(Utils::getTokenInLine(procStatusFile, 18, 2)) / 1000;
  return std::to_string(vmSizeMB);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::string procStatusFile =
      kProcDirectory + std::to_string(pid) + kStatusFilename;
  return Utils::getTokenInLine(procStatusFile, 9, 2);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
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

// TODO: Read and return the uptime of a process in seconds
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::string procStatFile =
      kProcDirectory + std::to_string(pid) + kStatFilename;
  vector<string> procStats = procStatTokens(pid);

  // start time of the process in jiffies
  long startTime = stol(procStats[ProcStates::starttime_]);

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