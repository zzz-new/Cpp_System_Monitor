#include <string>

#include "format.h"

using std::string;
using namespace std;

// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    int hour, minute;
    hour= seconds/3600;
    seconds=seconds % 3600;
    minute=seconds/60;
    seconds=seconds%60;

    string sec = to_string(seconds);
    sec.insert(0, 2-sec.length(), '0');

    string mins = to_string(minute);
    mins.insert(0, 2-mins.length(), '0');

    string hrs = to_string(hour);
    hrs.insert(0, 2-hrs.length(), '0');

    // return to_string(hour)+":"+
    //         to_string(minute)+":"+
    //         to_string(seconds);
    return sec+":"+mins+":"+hrs;
    // return string();
}