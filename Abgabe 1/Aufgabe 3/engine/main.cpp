// g++ -std=c++11 engine.cpp main.cpp -o test
// ./test

#include <iostream>
#include "engine.hpp"

int main()
{
    std::cout << add_pattern(1) << std::endl;
    std::cout << add_trajectory(1,1) << std::endl;
    std::cout << add_tuple(1,1,0,80) << std::endl;
    std::cout << add_tuple(1,1,10,90) << std::endl;
    std::cout << get_angle(1,1,0) << std::endl;
    std::cout << get_angle(1,1,6) << std::endl;
    std::cout << get_angle(1,1,10) << std::endl;
    return 0;
}
