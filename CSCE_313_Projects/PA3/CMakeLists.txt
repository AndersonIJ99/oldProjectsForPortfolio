cmake_minimum_required(VERSION 3.17)
project(PA3)

set(CMAKE_CXX_STANDARD 20)

add_executable(PA3 Reqchannel.h FIFOreqchannel.h FIFOreqchannel.cpp client.cpp server.cpp MQreqchannel.cpp MQreqchannel.h SHMreqchannel.cpp SHMreqchannel.h)