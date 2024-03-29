set(SOURCE
    ${SOURCE}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/event_queue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/checksum_worker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/crc32.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dir_traverser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/notifier.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/params_parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/syslogger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timeout_worker.cpp
)
set(HEADERS
    ${HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/event_queue.h
    ${CMAKE_CURRENT_SOURCE_DIR}/checksum_worker.h
    ${CMAKE_CURRENT_SOURCE_DIR}/crc32.h
    ${CMAKE_CURRENT_SOURCE_DIR}/notifier.h
    ${CMAKE_CURRENT_SOURCE_DIR}/params_parser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/syslogger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/timeout_worker.cpp

)
find_package(ZLIB REQUIRED)
find_package(jsoncpp REQUIRED)
add_executable(crc32daemon ${SOURCE} ${HEADERS} )
target_compile_features(crc32daemon PRIVATE cxx_std_17)

target_link_libraries(crc32daemon PRIVATE ZLIB::ZLIB jsoncpp_lib)

set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
