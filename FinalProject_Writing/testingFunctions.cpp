#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

// Function to parse date in MM/DD/YYYY format
tm parseDate(const std::string& date) {
    tm tm = {};
    istringstream ss(date);
    ss >> get_time(&tm, "%m/%d/%Y");
    return tm;
}

// Function to compare two dates
bool isDateInRange(const string& dateStr, const string& startStr, const string& endStr) {
    tm date = parseDate(dateStr);
    tm startDate = parseDate(startStr);
    tm endDate = parseDate(endStr);

    time_t dateT = mktime(&date);
    time_t startT = mktime(&startDate);
    time_t endT = mktime(&endDate);

    return (dateT >= startT && dateT <= endT);
}

int main() {
    string givenDate = "08/05/2023";
    string startDate = "07/01/2024";
    string endDate = "05/10/2025";

    if (isDateInRange(givenDate, startDate, endDate)) {
        cout << "The date " << givenDate << " is within the range " << startDate << " to " << endDate << ".\n" << endl;
    } else {
        cout << "The date " << givenDate << " is not within the range " << startDate << " to " << endDate << ".\n" << endl;
    }

    return 0;
}