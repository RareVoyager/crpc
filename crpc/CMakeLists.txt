# CONFIGURE_DEPENDS让cmake感知文件的变化
file(GLOB SRC_FILE CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
)

add_library(crpc STATIC
    ${SRC_FILE}
)

add_library(crpc::crpc ALIAS crpc)

target_include_directories(crpc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(crpc
    PUBLIC
        muduo::base
        muduo::net
        protobuf::libprotobuf
        zookeeper-client-c::zookeeper-client-c
)