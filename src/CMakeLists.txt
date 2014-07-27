set (SRC_LIST
    levmu_main.cc
    codec.cc
    levmuServer.cc
    levmuRequest.cc
    levmuResponse.cc
    )

link_directories(${PROJECT_SOURCE_DIR}/vendor/leveldb ${PROJECT_SOURCE_DIR}/vendor/build/release/lib)
add_executable(levmu ${SRC_LIST})
target_link_libraries(levmu leveldb muduo_base muduo_net pthread gmp)
