#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, token, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> token >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
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

float LinuxParser::MemoryUtilization() {
  string line;
  float memoryTotal{1.0}, memoryFree{1.0};
  string key, value, units;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> units) {
        if (key == "MemTotal:") {
          memoryTotal = std::stof(value);
        }
        if (key == "MemFree:") {
          memoryFree = std::stof(value);
        }
      }
    }
  }
  return (memoryTotal - memoryFree) / memoryTotal;
}

long LinuxParser::UpTime() {
  string line, upTimeStr;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTimeStr;
  }
  return std::stol(upTimeStr);
}

long LinuxParser::Jiffies() {
  string line;
  long user{0}, nice{0}, system{0}, idle{0}, iowait{0}, irq{0}, softirq{0},
      steal{0}, guest{0}, guest_nice{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>
        steal >> guest >> guest_nice;
  }
  return user + nice + system + idle + iowait + irq + softirq + steal + guest +
         guest_nice;
}

long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  vector<string> statData = {};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      statData.push_back(value);
    }
  }
  return std::stol(statData[13]) + std::stol(statData[14]) +
         std::stol(statData[15]) + std::stol(statData[16]);
}

long LinuxParser::ActiveJiffies() {
  string line;
  long user{0}, nice{0}, system{0}, idle{0}, iowait{0}, irq{0}, softirq{0},
      steal{0}, guest{0}, guest_nice{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >>
        steal >> guest >> guest_nice;
  }
  return user + nice + system + irq + softirq + steal + guest + guest_nice;
}

long LinuxParser::IdleJiffies() {
  string line;
  long user{0}, nice{0}, system{0}, idle{0}, iowait{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait;
  }
  return idle + iowait;
}

vector<string> LinuxParser::CpuUtilization() {
  string line, value;
  vector<string> cpuUtil;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::stringstream linestream(line);
    while (linestream >> value) {
      if (value != "cpu") {
        cpuUtil.push_back(value);
      }
    }
  }
  return cpuUtil;
}

int LinuxParser::TotalProcesses() {
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

int LinuxParser::RunningProcesses() {
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

string LinuxParser::Command(int pid) {
  string line;
  string command;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      std::replace(line.begin(), line.end(), ' ', '+');
      while (linestream >> command) {
        std::replace(line.begin(), line.end(), '+', ' ');
      }
      return command;
    }
  }
  return string();
}

string LinuxParser::Ram(int pid) {
  string line;
  string key, value;
  int vmsize{0};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          vmsize = std::stoi(value);
          vmsize = vmsize / 1000;
          return std::to_string(vmsize);
        }
      }
    }
  }
  return string();
}

string LinuxParser::Uid(int pid) {
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return string();
}

string LinuxParser::User(int pid) {
  string line;
  string user, x, uid;
  string uidTarget = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid) {
        if (uid == uidTarget) {
          return user;
        }
      }
    }
  }
  return string();
}

long LinuxParser::UpTime(int pid) {
  string line, value;
  long int starttime{0}, uptime{0};
  vector<string> statData = {};
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      statData.push_back(value);
    }
  }
  starttime = std::stol(statData[21]) / sysconf(_SC_CLK_TCK);
  uptime = LinuxParser::UpTime() - starttime;
  return uptime;
}