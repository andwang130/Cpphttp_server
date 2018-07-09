cmake_minimum_required(VERSION 3.10)
project(untitled5)

set(CMAKE_CXX_STANDARD 11)

add_executable(Cpphttp_server mian.cpp http_server.h http_server.cpp RequestHandler.h my_http_parser.h my_http_parser.cpp Application.h Application.cpp)