cmake_minimum_required(VERSION 3.10)
project(untitled5)

set(CMAKE_CXX_STANDARD 11)

set(-lmysqlclient)
include_directories(/usr/lib/x86_64-linux-gnu)
add_executable(Cpphttp_server mian.cpp http_server.h http_server.cpp RequestHandler.h my_http_parser.h my_http_parser.cpp Application.h Application.cpp RequestHandeler.cpp MyException.h umit.h umit.cpp MysqlDB.h MysqlDB.cpp)
target_link_libraries(Cpphttp_server libmysqlclient.so)