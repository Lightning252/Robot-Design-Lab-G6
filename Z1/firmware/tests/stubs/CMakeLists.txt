ADD_LIBRARY(stubs SHARED usart_stub.cpp various_stubs.cpp led_stub.cpp)
target_link_libraries(stubs pthread rt)



