#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


// TODO: Return this process's ID
int Process::Pid() { 
    return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    int utime, stime, cutime, cstime;
    float start, total, secs;
    string line, key, token;
    vector<string> values;

    std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);

    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream istream(line);

        while (istream >> token)
            values.push_back(token);
    }
        utime = std::stoi(values[13]);
        stime = std::stoi(values[14]);
        cutime = std::stoi(values[15]);
        cstime = std::stoi(values[16]);
        start = std::stoi(values[21]);

        total = float(utime + stime + cutime + cstime);
        secs = float (LinuxParser::UpTime() - (start / sysconf(_SC_CLK_TCK)));

        cpu_ = (total / sysconf(_SC_CLK_TCK)) / secs;

        return cpu_;
}

// TODO: Return the command that generated this process
string Process::Command() { 
    command_ = LinuxParser::Command(pid_);
    return command_; 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    ram_ = LinuxParser::Ram(pid_);
    return ram_; 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    user_ = LinuxParser::User(pid_);
    return user_; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    uptime_ = LinuxParser::UpTime(pid_); 
    return uptime_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return cpu_ > a.cpu_; 
}