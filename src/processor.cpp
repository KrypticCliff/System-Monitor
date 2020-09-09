#include "processor.h"
#include <unistd.h>

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    return (float)(LinuxParser::ActiveJiffies() / sysconf(_SC_CLK_TCK) / LinuxParser::UpTime()); 
}