#include <iostream>
#include "SomeAdd.hpp"
#include "SomeString.hpp"

int main() {
    std::string myString = returnString();
    std::cout << myString << std::endl;

    int sum = addTwoValues(5, 3);
    std::cout << "The sum is: " << sum << std::endl;

    return 0;
}