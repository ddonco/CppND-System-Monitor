#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  float Utilization();

 private:
  std::vector<std::string> cpuStringData = {};
  std::vector<float> cpuFloatData = {};
  float idle{0};
  float active{0};
  float prevActive{0};
  float total{0};
  float prevTotal{0};
  float dTotal{0};
  float dActive{0};
};

#endif