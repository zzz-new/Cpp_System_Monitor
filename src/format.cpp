#include <string>

#include "format.h"

using std::string;
using namespace std;

// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    float hour, minute;
    hour= seconds/3600;
    seconds=seconds % 3600;
    minute=seconds/60;
    seconds=seconds%60;

    return to_string(hour)+":"+
            to_string(minute)+":"+
            to_string(seconds);
}