// #include<iostream>
// #include<limits>

// int main () {
//     // std::cout << "Hello world!" << std::endl;
//     std::cout << "Hello world!\n";

//     int x {};
//     std::cout << x << std::endl;

//     std::cin.clear(); // resets any error flags
//     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignores character in the input buffer until we encounter a new line
//     std::cin.get(); // get one or more charcter from the user (waiting for the user to press enter)
//     return 0;
// }

#include <iostream>  // for std::cout and std::cin

int main()
{
    std::cout << "Enter two numbers: ";

    int x{};
    std::cin >> x;

    int y{};
    std::cin >> y;

    std::cout << "You entered " << x << " and " << y << '\n';

    return 0;
}