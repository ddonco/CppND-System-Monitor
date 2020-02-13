#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem()
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "PRETTY_NAME")
        {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel()
{
  string os, token, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> token >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr)
  {
    // Is this a directory?
    if (file->d_type == DT_DIR)
    {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit))
      {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  string line;
  long memoryTotal{0}, memoryFree{0};
  string titleMemTotal, titleMemFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open())
  {
    std::istringstream linestream;
    std::getline(filestream, line);
    linestream.str(line);
    linestream >> titleMemTotal >> memoryTotal;
    std::getline(filestream, line);
    linestream.str(line);
    linestream >> titleMemFree >> memoryFree;
  }
  return (float)(memoryTotal - memoryFree) / memoryTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime()
{
  string line;
  long upTime{0}, idleTime{0};
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
  string line;
  long user{0}, nice{0}, system{0}, idle{0}, iowait{0},
      irq{0}, softirq{0}, steal{0}, guest{0}, guest_nice{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  return user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)
{
  string line;
  int utime, stime, cutime, cstime;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i{1};
    while (i <= 22)
    {
      if (i == 14)
      {
        linestream >> utime >> stime >> cutime >> cstime;
      }
      i++;
    }
    // linestream >> pid_int >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> starttime;
  }
  return (long)(utime + stime + cutime + cstime);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  string line;
  long user{0}, nice{0}, system{0}, idle{0}, iowait{0},
      irq{0}, softirq{0}, steal{0}, guest{0}, guest_nice{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  return user + nice + system + irq + softirq + steal + guest + guest_nice;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  string line;
  long user{0}, nice{0}, system{0}, idle{0}, iowait{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait;
  }
  return idle + iowait;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
  string line, tag;
  vector<string> cpuUtil;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::stringstream sstream(line);
    sstream >> tag;
    std::istream_iterator<string> begin(sstream);
    std::istream_iterator<string> end;
    cpuUtil = vector<string>(begin, end);
    // std::copy(cpuUtil.begin(), cpuUtil.end(), std::ostream_iterator<string>(std::cout, "\n"));
  }
  return cpuUtil;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  string line;
  string key;
  int value{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "processes")
        {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  string line;
  string key;
  int value{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "procs_running")
        {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{
  string line;
  string command;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> command;
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "VmSize:")
        {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "Uid:")
        {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{
  string line;
  string user, x, uid;
  string uidTarget = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid)
      {
        if (uid == uidTarget)
        {
          return user;
        }
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{
  string line;
  string pid_int, comm, state, ppid, pgrp,
      session, tty_nr, tpgid, flags, minflt,
      cminflt, majflt, cmajflt, utime, stime,
      cutime, cstime, priority, nice, num_threads,
      itrealvalue, starttime;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int i{1};
    while (i <= 22)
    {
      linestream >> starttime;
      i++;
    }
    // linestream >> pid_int >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> starttime;
  }
  return (long)std::stoi(starttime);
}