#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using namespace std;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }


vector<Process>& System::Processes() {
    processes_.clear();
    vector<int> pids = LinuxParser::Pids();

    //auto pids = LinuxParser::Pids();
    for (int pid: pids){
        Process process(pid); // each Process contains pid, usr, cmd, etc.
        processes_.push_back(process);
    }
    std::sort(processes_.begin(),processes_.end());
    return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

// Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem(); 
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

// Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime();
}
