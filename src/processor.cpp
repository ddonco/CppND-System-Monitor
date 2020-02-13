#include <string>
#include <iostream>
#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization()
{
    // tag, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice
    cpuStringData = LinuxParser::CpuUtilization();
    // std::cout << "len: " << cpuStringData.size() << " - " << cpuStringData[8] << " " << cpuStringData[9] << std::endl;
    for (std::string s : cpuStringData)
    {
        cpuIntData.push_back(std::stoi(s));
    }

    prevIdle = idle;
    idle = cpuIntData[3] + cpuIntData[4];

    prevActive = active;
    active = cpuIntData[0] + cpuIntData[1] + cpuIntData[2] + cpuIntData[5] + cpuIntData[6] + cpuIntData[7];

    prevTotal = prevIdle + prevActive;
    total = idle + active;

    // differentiate: actual value minus the previous one
    dTotal = total - prevTotal;
    dIdle = idle - prevIdle;
    // std::cout << "util: " << total << " " << prevTotal << std::endl;
    return (float)(total - idle) / total;
}