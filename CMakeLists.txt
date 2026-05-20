cmake_minimum_required(VERSION 3.20)

project(crpc VERSION 0.0.1 LANGUAGES CXX)

# 使用C++17 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


find_package(protobuf CONFIG REQUIRED)
find_package(zookeeper-client-c CONFIG REQUIRED)