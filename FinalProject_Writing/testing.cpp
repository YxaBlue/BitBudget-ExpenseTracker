#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std;

// Function to get current time (Format: HH:MM AM/PM)
string getCurrentTime() {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    ostringstream oss;
    oss << put_time(ltm, "%I:%M %p");
    return oss.str();
}

int main() {
    cout << "The current time is: " << getCurrentTime() << endl;
    return 0;
}
