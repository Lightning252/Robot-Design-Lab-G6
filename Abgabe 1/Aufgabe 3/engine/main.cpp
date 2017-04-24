// g++ engine.cpp main.cpp -o test
// ./test

#include <iostream>
#include "engine.hpp"

int main()
{
	std::cout << add_tuple(1,2,0,50) << std::endl;
    std::cout << add_tuple(1,2,10,70) << std::endl;
    std::cout << add_tuple(1,2,30,80) << std::endl;
    std::cout << add_tuple(1,2,5,55) << std::endl;
    std::cout << get_angle(1,2,5) << std::endl;
    std::cout << get_angle(1,2,30) << std::endl;
    std::cout << get_angle(1,2,15) << std::endl;
    return 0;
}
