ADD_EXECUTABLE(testpc
  main.cpp
  CommunicationStub.cpp
  UARTCommunication.cpp
  ComProtocol.cpp
  ../stubs/IPCCom.cpp
  ../../crc.c
  ../../protocol.cpp)
target_link_libraries (testpc pthread rt)
