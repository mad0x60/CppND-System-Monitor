#include <algorithm>
#include <iostream>
#include <vector>

#include "ncurses_display.h"
#include "system.h"

// TODO: remove me
#include "format.h"
#include "linux_parser.h"
#include "process.h"
#include "processor.h"

int main() {
  System system;
  // std::vector<Process> processes;

  // std::cout << "cpu utilization: " << system.Cpu().Utilization();
  // processes = system.Processes();
  // for (const auto process : processes) {
  //   std::cout << process.Pid() << ", " << std::endl;
  // }
  // std::cout << "kernel: " << system.Kernel() << std::endl;
  // std::cout << "memory util: " << system.MemoryUtilization() << std::endl;
  // std::cout << "os: " << system.OperatingSystem() << std::endl;
  // std::cout << "running: " << system.RunningProcesses() << std::endl;
  // std::cout << "total proc: " << system.TotalProcesses() << std::endl;
  // std::cout << "uptime: " << system.UpTime() << std::endl;

  // std::cout << "format: " << Format::ElapsedTime(50) << std::endl;
  // std::cout << "format: " << Format::ElapsedTime(60) << std::endl;
  // std::cout << "format: " << Format::ElapsedTime(70) << std::endl;
  // std::cout << "format: " << Format::ElapsedTime(86399) << std::endl;
  // std::cout << "format: " << Format::ElapsedTime(86400) << std::endl;

  NCursesDisplay::Display(system);
}