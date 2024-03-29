project(tests LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
find_package(ZLIB REQUIRED)

add_executable(t tmain.cpp test.cpp test_crc.cpp ../app/params_parser.cpp ../app/crc32.cpp ../app/syslogger.cpp )
target_link_libraries(t PRIVATE gtest ZLIB::ZLIB)
add_test(NAME t COMMAND t)
