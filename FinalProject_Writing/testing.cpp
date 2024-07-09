#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// Helper function to convert MM/DD/YYYY to YYYYMMDD for easy comparison
std::string convertToComparableDate(const std::string& date) {
    std::string month = date.substr(0, 2);
    std::string day = date.substr(3, 2);
    std::string year = date.substr(6, 4);
    return year + month + day;
}

// Function to sort dates in ascending order
void sortDatesAscending(std::vector<std::string>& dates) {
    std::sort(dates.begin(), dates.end(), [](const std::string& a, const std::string& b) {
        return convertToComparableDate(a) < convertToComparableDate(b);
    });
}

// Function to sort dates in descending order
void sortDatesDescending(std::vector<std::string>& dates) {
    std::sort(dates.begin(), dates.end(), [](const std::string& a, const std::string& b) {
        return convertToComparableDate(a) > convertToComparableDate(b);
    });
}

int main() {
    // Example vector of dates in MM/DD/YYYY format
    std::vector<std::string> dates = {"07/09/2023", "12/25/2022", "01/01/2024", "05/15/2021"};
    
    // Sort dates in ascending order
    sortDatesAscending(dates);
    std::cout << "Dates in ascending order:" << std::endl;
    for (const auto& date : dates) {
        std::cout << date << std::endl;
    }
    
    // Sort dates in descending order
    sortDatesDescending(dates);
    std::cout << "Dates in descending order:" << std::endl;
    for (const auto& date : dates) {
        std::cout << date << std::endl;
    }
    
    return 0;
}
