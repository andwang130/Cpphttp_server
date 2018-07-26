cmake_minimum_required(VERSION 3.10)
project(untitled5)

set(CMAKE_CXX_STANDARD 11)

set(-lmysqlclient)
include_directories(/usr/lib/x86_64-linux-gnu)
add_executable(Cpphttp_server mian.cpp serever/http_server.h serever/http_server.cpp serever/RequestHandler.h serever/my_http_parser.h serever/my_http_parser.cpp serever/Application.h serever/Application.cpp serever/RequestHandeler.cpp serever/MyException.h serever/umit.h serever/umit.cpp serever/MysqlDB.h serever/MysqlDB.cpp)
target_link_libraries(Cpphttp_server libmysqlclient.so)