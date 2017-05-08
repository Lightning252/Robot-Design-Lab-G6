include_directories(../..)
ADD_EXECUTABLE(testrobot
	main.cpp
  	../../engine.cpp
	../../protocol.cpp
	../../crc.c
	../stubs/printf_stub.c)
target_link_libraries (testrobot stubs)


