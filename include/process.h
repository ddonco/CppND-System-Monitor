#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <string>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid = 0) : pid_(pid) { cpuUtil_ = this->CpuUtilization(); }
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  int pid_;
  float cpuUtil_{0};
};

#endif