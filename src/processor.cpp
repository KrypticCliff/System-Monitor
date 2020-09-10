#include "processor.h"
#include <unistd.h>

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  float total = LinuxParser::Jiffies();
  float active = LinuxParser::ActiveJiffies();
  float result = 1.0 * (active/total);

  return result;
}