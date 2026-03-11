cmake_minimum_required(VERSION 3.16)

project(myserver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(myserver mytcpserver.cpp
)

include(GNUInstallDirs)
install(TARGETS myserver
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
target_link_libraries(myserver PRIVATE ws2_32)
