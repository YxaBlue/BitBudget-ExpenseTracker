#include <iostream>
#include <string>

using namespace std;

bool isDouble(string str) {
    int dot = 0;
    if (str.empty()) return false;

    for (int i = 0; i < str.size(); i++) {
        if ((str[i] < '0') || (str[i] > '9'))
        {
            if (str[i] == '.') dot++;
            else return false;
        }
        if (dot > 1) return false;
    }

    if ((dot == 1) && (str.size() == 1)) return false;
    
    return true;
}


int main() {
    std::string test1 = "123.456";
    std::string test2 = ".123";
    std::string test3 = "123.";
    std::string test4 = "123.45.6";
    std::string test5 = "abc123";
    std::string test6 = "123";

    std::cout << std::boolalpha;
    std::cout << "Test1: " << isDouble(test1) << std::endl; // true
    std::cout << "Test2: " << isDouble(test2) << std::endl; // true
    std::cout << "Test3: " << isDouble(test3) << std::endl; // true
    std::cout << "Test4: " << isDouble(test4) << std::endl; // false
    std::cout << "Test5: " << isDouble(test5) << std::endl; // false
    std::cout << "Test6: " << isDouble(test6) << std::endl; // true

    return 0;
}