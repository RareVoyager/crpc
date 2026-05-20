# cmake_minimum_required(VERSION 3.20)

# project(crpc VERSION 0.0.1 LANGUAGES CXX)

# # 使用C++17 标准
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# # 让 CMake 在构建目录生成一个给工具使用的 compile_commands.json 文件。
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# # 设置可执行文件输出路径 ${CMAKE_BINARY_DIR} 是build目录
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# # 通过conan 下载的三方库 protobuf + zookeeper
# find_package(Threads REQUIRED)
# find_package(protobuf CONFIG REQUIRED)
# find_package(zookeeper-client-c CONFIG REQUIRED)

# # conan没有muduo库, 因此需要手动链接
# find_path(MUDUO_INCLUDE_DIR
#     NAMES muduo/net/TcpServer.h
#     PATHS usr/local/include
#     REQUIRED   
# )

# find_library(MUDUO_BASE_LIBRARY
#     NAMES muduo_base
#     PATHS usr/local/lib
#     REQUIRED
# )

# find_library(MUDUO_NET_LIBRARY 
#     NAMES muduo_net
#     PATHS usr/local/link
#     REQUIRED
# )

# add_library(muduo::base STATIC IMPORTED GLOBAL)
# set_target_properties(muduo::base PROPERTIES
#     IMPORTED_LOCATION "${MUDUO_BASE_LIBRARY}"
#     INTERFACE_INCLUDE_DIRECTORIES "${MUDUO_INCLUDE_DIR}"
#     INTERFACE_LINK_LIBRARIES Threads::Threads
# )
# add_library(muduo::net STATIC IMPORTED GLOBAL)
# set_target_properties(muduo::net PROPERTIES
#     IMPORTED_LOCATION "${MUDUO_NET_LIBRARY}"
#     INTERFACE_INCLUDE_DIRECTORIES "${MUDUO_INCLUDE_DIR}"
#     INTERFACE_LINK_LIBRARIES "muduo::base;Threads::Threads"
# )




# add_library(crpc::crpc ALIAS crpc)
# add_library(crpc::crpc ALIAS crpc)

# target_include_directories(crpc
#     PUBLIC
#         ${CMAKE_CURRENT_SOURCE_DIR}/crpc/include
# )

# target_link_libraries(crpc
#     PUBLIC
#         muduo::net
#         protobuf::libprotobuf
#         zookeeper-client-c::zookeeper-client-c
# )
# add_subdirectory(test)

cmake_minimum_required(VERSION 3.20)

project(crpc VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Threads REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(zookeeper-client-c CONFIG REQUIRED)

find_path(MUDUO_INCLUDE_DIR
    NAMES muduo/net/TcpServer.h
    PATHS /usr/local/include
    REQUIRED
)

find_library(MUDUO_BASE_LIBRARY
    NAMES muduo_base
    PATHS /usr/local/lib
    REQUIRED
)

find_library(MUDUO_NET_LIBRARY
    NAMES muduo_net
    PATHS /usr/local/lib
    REQUIRED
)

add_library(muduo::base STATIC IMPORTED GLOBAL)
set_target_properties(muduo::base PROPERTIES
    IMPORTED_LOCATION "${MUDUO_BASE_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${MUDUO_INCLUDE_DIR}"
    INTERFACE_LINK_LIBRARIES Threads::Threads
)

add_library(muduo::net STATIC IMPORTED GLOBAL)
set_target_properties(muduo::net PROPERTIES
    IMPORTED_LOCATION "${MUDUO_NET_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${MUDUO_INCLUDE_DIR}"
    INTERFACE_LINK_LIBRARIES "muduo::base;Threads::Threads"
)

add_library(crpc STATIC
    crpc/src/example.cc
)

add_library(crpc::crpc ALIAS crpc)

target_include_directories(crpc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/crpc/include
)

target_link_libraries(crpc
    PUBLIC
        muduo::net
        protobuf::libprotobuf
        zookeeper-client-c::zookeeper-client-c
)

add_subdirectory(test)
