// g++ engine.cpp main.cpp -o test
// ./test

#include <iostream>
#include "engine.hpp"

int main()
{
	std::cout << (add_tuple(0,0,0,10)==0) << std::endl;
    std::cout << (add_tuple(0,0,100,20)==0) << std::endl;
	std::cout << (add_tuple(0,0,200,30)==0) << std::endl;
	std::cout << (add_tuple(0,0,300,40)==0) << std::endl;
	std::cout << (add_tuple(0,0,300,50)==-2) << std::endl;
	std::cout << (add_tuple(-1,0,300,50)==-1) << std::endl;
	std::cout << (get_angle(0,0,-10)==-1) << std::endl;
    std::cout << (get_angle(0,0,0)==10) << std::endl;
	std::cout << (get_angle(0,0,100)==20) << std::endl;
	std::cout << (get_angle(0,0,200)==30) << std::endl;
	std::cout << (get_angle(0,0,300)==40) << std::endl;
	std::cout << (get_angle(0,0,120)==22) << std::endl;
	std::cout << (get_angle(0,0,150)==25) << std::endl;
	std::cout << (get_angle(0,0,180)==28) << std::endl;
	return 0;
}
