#include "ncurses_display.h"
#include "system.h"

#include <iostream>

// TODO: remove me
#include "linux_parser.h" 

int main() {
  System system;

  int pid = 1;

  std::cout << "OperatingSystem: " << LinuxParser::OperatingSystem() << std::endl;
  std::cout << "Kernel: " << LinuxParser::Kernel() << std::endl;

  for (auto pid : LinuxParser::Pids())
  {
    std::cout << "" << pid << ", ";  
  }
  std::cout << std::endl;

  std::cout << "MemoryUtilization: " << LinuxParser::MemoryUtilization() << std::endl;
  std::cout << "UpTime: " << LinuxParser::UpTime() << std::endl;
  std::cout << "Jiffies: " << LinuxParser::Jiffies() << std::endl;
  std::cout << "ActiveJiffies: " << LinuxParser::ActiveJiffies() << std::endl;
  std::cout << "IdleJiffies: " << LinuxParser::IdleJiffies() << std::endl;
  std::cout << "CpuUtilization: " << LinuxParser::CpuUtilization()[0] << std::endl;
  std::cout << "TotalProcesses: " << LinuxParser::TotalProcesses() << std::endl;
  std::cout << "RunningProcesses: " << LinuxParser::RunningProcesses() << std::endl;
  std::cout << "ActiveJiffies: " << LinuxParser::ActiveJiffies(pid) << std::endl;
  std::cout << "Command: " << LinuxParser::Command(pid) << std::endl;
  std::cout << "Ram: " << LinuxParser::Ram(pid) << std::endl;
  std::cout << "Uid: " << LinuxParser::Uid(pid) << std::endl;
  std::cout << "User: " << LinuxParser::User(pid) << std::endl;
  std::cout << "UpTime: " << LinuxParser::UpTime(pid) << std::endl;
  // NCursesDisplay::Display(system);
}