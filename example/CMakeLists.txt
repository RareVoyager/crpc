include(${CMAKE_SOURCE_DIR}/cmake/utils.cmake)


crpc_example(userservice callee/userservice.cc user.pb.cc)
crpc_example(caller caller/callerservice.cc user.pb.cc)