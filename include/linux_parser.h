#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <vector>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

// Process
enum ProcStates {
  pid_ = 0,
  comm_,
  state_,
  ppid_,
  pgrp_,
  session_,
  tty_nr_,
  tpgid_,
  flags_,
  minflt_,
  cminflt_,
  majflt,
  cmajflt_,
  utime_,
  stime_,
  cutime_,
  cstime_,
  priority_,
  nice_,
  num_threads_,
  itrealvalue_,
  starttime_,
  vsize_,
  rss_,
  rsslim_,
  startcode_,
  endcode_,
  startstack_,
  kstkesp_,
  kstkeip_,
  signal_,
  blocked_,
  sigignore_,
  sigcatch_,
  wchan_,
  nswap_,
  cnswap_,
  exit_signal_,
  processor_,
  rt_priority_,
  policy_,
  delayacct_blkio_ticks_,
  guest_time_,
  cguest_time_,
  start_data_,
  end_data_,
  start_brk_,
  arg_start_,
  arg_end_,
  env_start_,
  env_end_,
  exit_code_
};

// System
/**
 *
 * @return float memory utilization of the system
 */
float MemoryUtilization();

/**
 *
 * @return long The uptime of the system
 */
long UpTime();

/**
 *
 * @return std::vector<int> of the current processes of the system
 */
std::vector<int> Pids();

/**
 *
 * @return int the total number of processes in the system
 */
int TotalProcesses();

/**
 *
 * @return int the number of running processes
 */
int RunningProcesses();

/**
 *
 * @return std::string of the current operating system
 */
std::string OperatingSystem();

/**
 *
 * @return std::string of the kernel of the running operating system
 */
std::string Kernel();

/**
 *
 * @return std::vector<std::string> of cpu stats from the /proc/stat file
 */
std::vector<std::string> CpuUtilization();

/**
 *
 * @return long the number of passed jiffies of the system
 */
long Jiffies();

/**
 *
 * @return long the number of active jiffies of the system
 */
long ActiveJiffies();

/**
 *
 * @return long the number of idle jiffies of the system
 */
long IdleJiffies();

// Processes
/**
 * @param pid process id
 * @return std::string the command used to run the process with process id pid
 */
std::string Command(int pid);

/**
 * @param pid process id
 * @return std::string the RAM usage of the process with process id pid
 */
std::string Ram(int pid);

/**
 * @param pid process id
 * @return std::string the user id of the process with process id pid
 */
std::string Uid(int pid);

/**
 * @param pid process id
 * @return std::string the username of the process with process id pid
 */
std::string User(int pid);

/**
 * @param pid process id
 * @return long the uptime of the process with process id pid
 */
long UpTime(int pid);

/**
 *
 * @param pid process id
 * @return long the number of active jiffies of the process with process id pid
 */

/**
 * @param pid process id
 * @return long the number of active jiffies of the process with process id pid
 */
long ActiveJiffies(int pid);

/**
 * @param pid process id
 * @return std::vector<std::string> a vector of strings of the tokens of the
 * /proc/[pid]/stat file
 */
std::vector<std::string> procStatTokens(int pid);
};  // namespace LinuxParser

#endif