cmake_minimum_required(VERSION 3.10)
project(untitled5)

set(CMAKE_CXX_STANDARD 11)
include_directories(/usr/lib/x86_64-linux-gnu)
include_directories(/usr/include/mysql)
add_executable(Cpphttp_server mian.cpp serever/http_server.h serever/http_server.cpp serever/RequestHandler.h serever/my_http_parser.h serever/my_http_parser.cpp serever/Application.h serever/Application.cpp serever/RequestHandeler.cpp serever/MyException.h serever/umit.h serever/umit.cpp  serever/my_websocket_parser.cpp serever/my_websocket_parser.h
        serever/base64.h serever/base64.cpp serever/SHA1.h serever/WebsokcetHandelr.h serever/WebsocketHandelr.cpp myhandel/my_handelr.h serever/CHTTP.cpp serever/CHTTP.h serever/_include.h serever/ThreadPool.hpp DB/mysqll++test.h)
#添加一个当前路径，这样就可以使用相对路径
#link_directories(lib)
target_link_libraries(Cpphttp_server libmysqlpp.so)#会在lib路径找到
##libmysqlclient.so.20: 无法添加符号: DSO missing from command line错误 ，知道这个连接库解决了
target_link_libraries(Cpphttp_server /usr/lib/x86_64-linux-gnu/libmysqlclient.so.20)
find_package(Threads REQUIRED)
target_link_libraries(Cpphttp_server Threads::Threads)
