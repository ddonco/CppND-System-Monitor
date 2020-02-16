#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long secs) {
  string hours, minutes, seconds;
  long int numZeros = 2;
  hours = std::to_string(secs / 3600);
  secs = secs % 3600;
  minutes = std::to_string(secs / 60);
  secs = secs % 60;
  seconds = std::to_string(secs);
  return string(numZeros - hours.length(), '0').append(hours) + ":" +
         string(numZeros - minutes.length(), '0').append(minutes) + ":" +
         string(numZeros - seconds.length(), '0').append(seconds);
}