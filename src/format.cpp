#include <string>
#include <iomanip>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    std::ostringstream ostream;

    int hours;
    int minutes;

    hours = seconds / 3600;
    seconds %= 3600;
    minutes = seconds / 60;
    seconds %= 60;

    ostream << std::setw(2) << std::setfill('0') << hours << ":"
            << std::setw(2) << std::setfill('0') << minutes << ":"
            << std::setw(2) << std::setfill('0') << seconds;

    return ostream.str(); 
}
