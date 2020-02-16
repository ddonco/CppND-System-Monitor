#include "processor.h"
#include <iostream>
#include <string>
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  // tag, user, nice, system, idle, iowait, irq, softirq, steal, guest,
  // guest_nice
  cpuStringData = LinuxParser::CpuUtilization();
  cpuFloatData = {};
  for (unsigned int i = 0; i < cpuStringData.size(); i++) {
    cpuFloatData.push_back(std::stof(cpuStringData[i]));
  }

  idle = cpuFloatData[3] + cpuFloatData[4];

  active = cpuFloatData[0] + cpuFloatData[1] + cpuFloatData[2] +
           cpuFloatData[5] + cpuFloatData[6] + cpuFloatData[7];

  total = idle + active;

  // differentiate: actual value minus the previous one
  dTotal = total - prevTotal;
  dActive = active - prevActive;
  prevTotal = total;
  prevActive = active;

  return dActive / dTotal;
}