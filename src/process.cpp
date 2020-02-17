#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return this->pid_; }

float Process::CpuUtilization() {
  const long int totaltime = LinuxParser::ActiveJiffies(this->pid_);
  const long int uptime = LinuxParser::UpTime(this->pid_);
  return (float)(totaltime / sysconf(_SC_CLK_TCK)) / uptime;
}

string Process::Command() { return LinuxParser::Command(this->pid_); }

string Process::Ram() { return LinuxParser::Ram(this->pid_); }

string Process::User() { return LinuxParser::User(this->pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(this->pid_); }

bool Process::operator<(Process const& a) const {
  return this->cpuUtil_ > a.cpuUtil_;
}