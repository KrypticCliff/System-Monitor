#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    // Linux Version (Kernel) <- Catching the kernel
    for (int i = 0; i < 3; i++) {
      linestream >> kernel;
    }
    // linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string line;
  std::string key;
  float value;

  float memTotal = 0;
  float memFree = 0;
  float buffers = 0;

  // Double '/' may cause explosion
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::remove(line.begin(), line.end(), ':');

      std::istringstream istream(line);
      istream >> key >> value;

      if (key == "MemTotal")
        memTotal = value;
      else if (key == "MemFree")
        memFree = value;
      else if (key == "Buffers")
        buffers = value;
    }
  }
  // Shows high memory usage
  // return ((memTotal - memFree) / memTotal);
  return 1 - (memFree / (memTotal - buffers));
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  std::string line;
  long value;

  // Double '/' may cause explosion
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  // TESTING:
  // if (stream.is_open()) {
  //   if (std::getline(stream, line)) {
  //     std::istringstream istream(line);
  //     istream >> value;
  //   }
  //   return value;
  // }

  if (stream.is_open()) {
    std::getline(stream, line);

    std::istringstream istream(line);
    istream >> value;

    return value;
  }

  return value;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::UpTime()*sysconf(_SC_CLK_TCK);
  // std::string line;
  // std::string key;
  // long value = 0;

  // // Double '/' may cause explosion
  // std::ifstream stream(kProcDirectory + kStatFilename);

  // if (stream.is_open()) {
  //   getline(stream, line);
  //   std::istringstream istream(line);
  //   istream >> key;

  //   if (key == "cpu") {
  //     for (int i = 0; i < 10; i++) {
  //       long ticks;

  //       istream >> ticks;
  //       value += ticks;
  //     }
  //     return value;
  //   }
  // }
  // return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { 
  // std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  // long value;
  // string line, stime, cstime, cutime;

  // if (stream.is_open()) {
  //   int count = 1;
  //   string temp;

  //   getline(stream, line);
  //   std::istringstream istream(line);

  //   while(count <= 13) {
  //     istream >> temp;
  //     count++;
  //   }

  //   istream >> stime >> stime >> cutime >> cstime;
  // }
  // value = std::stol(stime) + std::stol(stime) +
  //         std::stol(cutime) + std::stol(cstime);
  // return value;
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> util = LinuxParser::CpuUtilization();
  long ajiff = 0;
  ajiff = stol(util[0]) + stol(util[1]) + stol(util[2]) + stol(util[5]) + stol(util[6])
  + stol(util[7]) + stol(util[8]) + stol(util[9]);

  return ajiff; 
  // std::ifstream stream(kProcDirectory + kStatFilename);

  // if (stream.is_open()) {
  //   string line, key;
  //   int kNice_, kUser_, kIRQ_, kSoftIRQ_, kSystem_, kSteal_;
  //   int temp;


  //   getline(stream, line);
  //   std::istringstream istream(line);

  //   istream >> key;
  //   if (key == "cpu") {
  //     istream >> kUser_ >> kNice_ >> kSystem_;
  //     istream >> temp >> temp;
  //     istream >> kIRQ_ >> kSoftIRQ_ >> kSteal_;
  //   }
  //   return kUser_ + kNice_ + kSystem_ + kIRQ_ + kSoftIRQ_ + kSteal_;
  // }
  // return 0;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> util = LinuxParser::CpuUtilization();
  long ijiff = 0;
  ijiff = stol(util[3]) + stol(util[4]);

  return ijiff;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::string line, key, token;
  vector<string> values;

  std::ifstream stream("/proc/stat");

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream istream(line);

    while (istream >> key) {
      if (key == "cpu") {
        while (istream >> token) 
          values.push_back(token);

        return values;
      }
    }
  }
  return values;
      
      // for (int i = 0; i < 6; i++) {
      //   long ticks;
      //   istream >> ticks;

      //   if (i == 4 || i == 5) {
      //     value += ticks;
      //   }
      // }
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line;
  std::string key;
  int value;

  // Double '/' may cause explosion
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream istream(line);

      istream >> key >> value;

      if (key == "processes") return value;
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string line, key;
  int value;

  // Double '/' may cause explosions
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (getline(stream, line)) {
      std::istringstream istream(line);

      istream >> key >> value;

      if (key == "procs_running") return value;
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid)+ kCmdlineFilename);

  if (stream.is_open())
    std::getline(stream, line);

  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) {
  std::string line, key, value, ram;
  std::ifstream stream(kProcDirectory + to_string(pid)+ kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream istream(line);

      while (istream >> key >> value) {
        if (key == "VmData:") {
          ram = value;
          return to_string(stoi(ram)/1024);
        }
      }
    }
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::string key;
  std::string value;
  std::string line;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');

      std::istringstream linestream(line);
      linestream >> key >> value;

      if (key == "Uid") return value;
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::string line, user, filler;
  int uid;

  std::ifstream stream(kPasswordPath);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');

      std::istringstream istream(line);

      istream >> user >> filler >> uid;
      if (uid == pid) {
        return user;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long int LinuxParser::UpTime(int pid) {
  int count = 0;
  std::string line, key, token;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream istream(line);

    while (istream >> key && count < 22)
      ++count;
  }
  
  return std::stol(key);
}