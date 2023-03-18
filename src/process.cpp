#include "process.h"

#include <unistd.h>

#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  pid_ = pid;
  cmd_ = LinuxParser::Command(pid);
  ram_ = LinuxParser::Ram(pid);
  user_ = LinuxParser::User(pid);
  uptime_ = LinuxParser::UpTime(pid);

  long delUptime = LinuxParser::UpTime() - uptime_;
  long totaltime = LinuxParser::ActiveJiffies(pid);
  cpu_ = float(totaltime) / float(delUptime);
}

// putting in const somehow works

// Return this process's ID
int Process::Pid() const { 
    return pid_;
}

// Return this process's CPU utilization
float Process::CpuUtilization() const { 
    return cpu_;
}

// Return the command that generated this process
string Process::Command() const { 
    return cmd_; 
}

// Return this process's memory utilization
string Process::Ram() const { 
    return ram_;
}

// Return the user (name) that generated this process
string Process::User() const {
    return user_;
}

//  Return the age of this process (in seconds)
long int Process::UpTime() const { 
    return uptime_;
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return CpuUtilization() > a.CpuUtilization();
}