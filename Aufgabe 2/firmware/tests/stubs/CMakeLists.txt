ADD_LIBRARY(stubs SHARED
  usart_stub.cpp
  various_stubs.cpp
  led_stub.cpp
  IPCCom.cpp
  printf_stub.c)
target_link_libraries(stubs pthread rt)



