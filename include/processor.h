#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor
{
public:
    float Utilization(); // TODO: See src/processor.cpp

    // TODO: Declare any necessary private members
private:
    std::vector<std::string> cpuStringData = {};
    std::vector<int> cpuIntData = {};
    int idle{0};
    int prevIdle{0};
    int active{0};
    int prevActive{0};
    int total{0};
    int prevTotal{0};
    int dTotal{0};
    int dIdle{0};
};

#endif