#include <iostream>

#include "ncurses_display.h"
#include "system.h"

// TODO: remove me
#include "linux_parser.h"
#include "process.h"

int main() {
  System system;

  int pid = 200089;

  Process process(pid);

  std::cout << "pid: " << process.Pid() << std::endl;
  std::cout << "CpuUtilization: " << process.CpuUtilization() << std::endl;
  std::cout << "Command: " << process.Command() << std::endl;
  std::cout << "Ram: " << process.Ram() << std::endl;
  std::cout << "User: " << process.User() << std::endl;
  std::cout << "UpTime: " << process.UpTime() << std::endl;

  // NCursesDisplay::Display(system);
}