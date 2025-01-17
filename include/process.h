#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
*/
class Process {
 public:
  Process(int id) : pid(id) {}
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;

 private:
  int pid;
};

#endif