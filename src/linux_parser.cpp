#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

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

// Read and return the system memory utilization from '/proc/meminfo'
float LinuxParser::MemoryUtilization() { 
  string line, key;
  string memTotal_s, memFree_s;
  float memUsage;
  // float memTotal, memFree, memUsage;
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){ // get each line
      std::istringstream linestream(line);
      // parse the line into separate words and extract the key words
      linestream >> key; 

      if (key=="MemTotal:"){
        linestream >> memTotal_s;
      }
      else if (key=="MemFree:"){
        linestream >> memFree_s;
        break; // without break, it keeps looping through all keys
      }
    }
    //memUsage = (memTotal-memFree)/memTotal;
    memUsage = (stof(memTotal_s)-stof(memFree_s))/stof(memTotal_s);
  }
  return memUsage;
}

// Read and return the system uptime from /proc/uptime
long LinuxParser::UpTime() {
  long uptime;
  string line, value;

  std::ifstream stream(kProcDirectory+kUptimeFilename);// only 1 line
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
    uptime= stol(value);
  }
  return uptime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies()+LinuxParser::IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  long active=0;
  string line, value;
  vector<string> values;

  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value){
      values.push_back(value);
    }     
  }

  long utime = stol(values[13]);
  long stime = stol(values[14]);
  long cutime = stol(values[15]);
  long cstime = stol(values[16]);
  active = utime + stime+ cutime + cstime;
  
  return active/sysconf(_SC_CLK_TCK);
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long active=0;
  vector<string> cpu = LinuxParser::CpuUtilization();

  for (auto i: cpu){
    active += stol(i);
  }
  return active;
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpu = LinuxParser::CpuUtilization();

  return stol(cpu[3]); // the 4th value is kIdle_
}

// Read and return CPU utilization from /proc/stat file
vector<string> LinuxParser::CpuUtilization() {
  string line, key;
  vector<string> cpu_list;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){ // get each line
      std::istringstream linestream(line);
      linestream >> key;
      if (key=="cpu"){
          while (linestream>>key){
            cpu_list.push_back(key);
          }
        break;
      }
    }
  }
  return cpu_list;
}

// Read and return the total number of processes from /proc/stat
int LinuxParser::TotalProcesses() {
  string line, key;
  int totalProcess;

  std::ifstream stream(kProcDirectory+kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){ // get each line
      std::istringstream linestream(line);
      linestream >> key; 

      if (key=="processes"){
        linestream >> totalProcess;
        break;
      }
    }
  }
  return totalProcess;
}

// Read and return the number of running processes /proc/stat
int LinuxParser::RunningProcesses() { 
  string line, key;
  int runningProcess;

  std::ifstream stream(kProcDirectory+kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){ // get each line
      std::istringstream linestream(line);
      linestream >> key; 

      if (key=="procs_running"){
        linestream >> runningProcess;
        break;
      }
    }
  }
  return runningProcess;
}

// Read and return the command associated with a process from /proc/[pid]/cmdline
string LinuxParser::Command(int pid) {
  string line;

  std::ifstream stream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);      
  }
  return line;
}

// Read and return the memory used by a process from /proc/[pid]/stat
string LinuxParser::Ram(int pid) {
  string line, key, vmsize;
  long ram;

  std::ifstream stream(kProcDirectory+to_string(pid)+kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){ // get each line
      std::istringstream linestream(line);
      linestream >> key >> vmsize;
      if (key=="VmSize:"){
        ram=stol(vmsize)/1024.;
      }
    }
  }
  return to_string(ram);
}

// Read and return the user ID associated with a process /proc/[pid]/stat
string LinuxParser::Uid(int pid) {
  string line, key, uid;

  std::ifstream stream(kProcDirectory+to_string(pid)+kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){ // get each line
      std::istringstream linestream(line);
      linestream >> key;
      if (key=="Uid:"){
        linestream >> uid;
        break;
      }
    }
  }
  return uid;
}

// Read and return the user associated with a process from /proc/[pid]/stat
// Give a pid, returns a uid, and then the corresponding username
string LinuxParser::User(int pid) {
  string line, uid;
  uid=LinuxParser::Uid(pid);
  
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while (std::getline(stream, line)){ // get each line
      size_t found = line.find(":"+uid+":"); // find substring, stores position
      if (found!= string::npos){ // if found, (found != -1) = True
        return line.substr(0, line.find(":") );
      }
    }
  }
  return "";
}

// Read and return the uptime of a process from /proc/[pid]/stat
// According to proc man(5), the 22nd item is the starttime
long LinuxParser::UpTime(int pid) {
  string line, value;
  long starttime=0;

  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){ // get each line
      std::istringstream linestream(line);
      
      int i=1;
      while (linestream >> value){
        if (i==22){
          starttime= stol(value)/sysconf(_SC_CLK_TCK);
          break;
        }
        i+=1;
      }
    }
  }

  long uptime = LinuxParser::UpTime();
  return uptime-starttime;
}