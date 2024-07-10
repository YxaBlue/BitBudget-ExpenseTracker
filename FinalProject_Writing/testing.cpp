#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// // Helper function to convert MM/DD/YYYY to YYYYMMDD for easy comparison
// std::string convertToComparableDate(const std::string& date) {
//     std::string month = date.substr(0, 2);
//     std::string day = date.substr(3, 2);
//     std::string year = date.substr(6, 4);
//     return year + month + day;
// }

// // Function to sort dates in ascending order
// void sortDatesAscending(std::vector<std::string>& dates) {
//     std::sort(dates.begin(), dates.end(), [](const std::string& a, const std::string& b) {
//         return convertToComparableDate(a) < convertToComparableDate(b);
//     });
// }

// // Function to sort dates in descending order
// void sortDatesDescending(std::vector<std::string>& dates) {
//     std::sort(dates.begin(), dates.end(), [](const std::string& a, const std::string& b) {
//         return convertToComparableDate(a) > convertToComparableDate(b);
//     });
// }

// int main() {
//     // Example vector of dates in MM/DD/YYYY format
//     std::vector<std::string> dates = {"07/09/2023", "12/25/2022", "01/01/2024", "05/15/2021"};
    
//     // Sort dates in ascending order
//     sortDatesAscending(dates);
//     std::cout << "Dates in ascending order:" << std::endl;
//     for (const auto& date : dates) {
//         std::cout << date << std::endl;
//     }
    
//     // Sort dates in descending order
//     sortDatesDescending(dates);
//     std::cout << "Dates in descending order:" << std::endl;
//     for (const auto& date : dates) {
//         std::cout << date << std::endl;
//     }
    
//     return 0;
// }



#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

string makePercentBar(double percentage, int barWidth = 50) {
    // Clamp the percentage between 0 and 100
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;

    // Create bar
    int filledWidth = static_cast<int>(barWidth * (percentage / 100.0));
    string bar(barWidth, '-');
    for (int i = 0; i < filledWidth; ++i) bar[i] = '#';

    // Convert double to string, displaying 2 decimal places
    stringstream ss;
    ss << fixed << setprecision(0) << percentage;

    // Combine bar and percentage
    string percentBar = "[" + bar + "] " + ss.str() + "%";

    return percentBar;
}

int main() {
    cout << makePercentBar(75.5) << endl;   // Example usage: display 75.5% filled bar
    cout << makePercentBar(40) << endl;     // Example usage: display 40% filled bar
    cout << makePercentBar(100) << endl;    // Example usage: display 100% filled bar
    cout << makePercentBar(0) << endl;      // Example usage: display 0% filled bar
    cout << makePercentBar(-10) << endl;    // Example usage: clamped to 0%
    cout << makePercentBar(120) << endl;    // Example usage: clamped to 100%
    return 0;
}
