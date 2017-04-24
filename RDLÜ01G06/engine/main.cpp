// g++ engine.cpp main.cpp -o test
// ./test

#include <iostream>
#include "engine.hpp"

//tests
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

/*
output:

1
1
1
1
1
1
1
1
1
1
1
1
1

valgrind:
==2711== 
==2711== HEAP SUMMARY:
==2711==     in use at exit: 0 bytes in 0 blocks
==2711==   total heap usage: 2 allocs, 2 frees, 73,728 bytes allocated
==2711== 
==2711== All heap blocks were freed -- no leaks are possible
==2711== 
==2711== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
==2711== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/

