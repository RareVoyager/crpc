function(crpc_test target_name)
    add_executable(${target_name}
        ${ARGN}
    )

    target_include_directories(${target_name}
        PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_link_libraries(${target_name}
        PRIVATE
            crpc::crpc
    )
endfunction()


function(crpc_example target_name)
    add_executable(${target_name}
        ${ARGN}
    )

    target_include_directories(${target_name}
        PRIVATE
            ${CMAKE_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_link_libraries(${target_name}
        PRIVATE
            crpc::crpc
    )
endfunction()
