# cmake最小版本
cmake_minimum_required(VERSION 3.20)

# 项目名
project(crpc VERSION 0.0.1 LANGUAGES CXX)

# C++17 标准 强制C++17 标准不允许降级 不允许使用额外拓展
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# 生成一个json编译文件
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 可执行文件生成的目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 找到Thread protobuf zookeeper库
find_package(Threads REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(zookeeper-client-c CONFIG REQUIRED)

# 手动链接三方库 muduo
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


add_subdirectory(crpc)

add_subdirectory(test)

add_subdirectory(example)
