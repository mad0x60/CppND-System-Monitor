#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>

#include "linux_parser.h"

#include "utils.h"

using std::stof;
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
  long memTotal =  std::stol(Utils::getTokenInLine(kProcDirectory + kMeminfoFilename, 1, 2));
  long memFree =  std::stol(Utils::getTokenInLine(kProcDirectory + kMeminfoFilename, 2, 2));
  return (float)(memTotal - memFree) / memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  return std::stol(Utils::getTokenInLine(kProcDirectory + kUptimeFilename, 1, 1));  
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long totalJiffies = 0;
  string cpuStatStr = Utils::getLine(kProcDirectory + kStatFilename, 1);
  std::istringstream cpuStat(cpuStatStr);
  cpuStat.ignore(5, ' '); // ignoring the cpu token
  for (string jiffies; cpuStat >> jiffies; totalJiffies += stol(jiffies));
  return totalJiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return Jiffies() - IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long idleTime = std::stol(Utils::getTokenInLine(kProcDirectory + kStatFilename, 1, 5));  
  long ioWaitTime = std::stol(Utils::getTokenInLine(kProcDirectory + kStatFilename, 1, 6)); 
  return idleTime + ioWaitTime;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  return { std::to_string((float)(ActiveJiffies())/Jiffies()) };
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return std::stol(Utils::getTokenInLine(kProcDirectory + kStatFilename, 9, 2));  
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return std::stol(Utils::getTokenInLine(kProcDirectory + kStatFilename, 10, 2));   
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string procStatFile = kProcDirectory + std::to_string(pid) + "/" + kStatFilename;
  return std::stol(Utils::getTokenInLine(procStatFile, 1, 14));
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string procCmdFile = kProcDirectory + std::to_string(pid) + "/" + kCpuinfoFilename;
  return Utils::getLine(procCmdFile, 1);
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string procStatusFile = kProcDirectory + std::to_string(pid) + "/" + kStatusFilename;

  // divide by 1000 to convert from KB to MB
  long vmSizeMB =  std::stol(Utils::getTokenInLine(procStatusFile, 18, 2)) / 1000;
  return std::to_string(vmSizeMB);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
    std::string procStatusFile = kProcDirectory + std::to_string(pid) + "/" + kStatusFilename;
    return Utils::getTokenInLine(procStatusFile, 9, 2);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = Uid(pid); // string of UID

  string line;
  std::vector<string> tokens;
  std::ifstream filestream (kPasswordPath); // /etc/passwd file
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {

      // tokenizing
      for(auto token = strtok(&line[0], ":"); token != NULL; token = strtok(NULL, ":")) {
        if (uid.compare(token) == 0) {
          return tokens[0];
        } else {
          tokens.push_back(token);
        }
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  std::string procStatFile = kProcDirectory + std::to_string(pid) + "/" + kStatFilename;
  return std::stol(Utils::getTokenInLine(procStatFile, 1, 14));
}