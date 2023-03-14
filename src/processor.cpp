#include "processor.h"
#include "linux_parser.h"
#include <chrono>
#include <thread>

// Return the aggregate CPU utilization

// float Processor::Utilization() {
//     long prevTotalJiffy, prevIdleJiffy, currTotalJiffy, currIdleJiffy;
//     float delTotalJiffy, delIdleJiffy;

//     prevTotalJiffy = LinuxParser::Jiffies();
//     prevIdleJiffy = LinuxParser::IdleJiffies();

//     // https://stackoverflow.com/questions/3017162/how-to-get-total-cpu-usage-in-linux-using-c
//     std::this_thread::sleep_for(std::chrono::milliseconds(1000));

//     currTotalJiffy = LinuxParser::Jiffies();
//     currIdleJiffy = LinuxParser::IdleJiffies();

//     delTotalJiffy = float(currTotalJiffy) - float(prevTotalJiffy);
//     delIdleJiffy = float(currIdleJiffy) - float(prevIdleJiffy);

//     return (delTotalJiffy-delIdleJiffy)/delTotalJiffy;  
// }

float Processor::Utilization() {
    return float(LinuxParser::ActiveJiffies()/LinuxParser::Jiffies());
}