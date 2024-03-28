cmake_minimum_required(VERSION 3.25...3.28)
project(
    crc32demon
    VERSION 0.1
    LANGUAGES CXX)

include(CTest)
enable_testing()

include_directories(app)
add_subdirectory(app)
add_subdirectory(tests)
