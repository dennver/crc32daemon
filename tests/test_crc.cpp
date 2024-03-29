#include "../app/crc32.h"

#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <zlib.h>

int populateFile(std::string const& file)
{
    std::ofstream myfile(file);
    if (!myfile)
       return -1;

    for (int index = 0; index < 8192; ++index)
        myfile << (rand()%1000)+1 << "\n";
    return 0;
}

TEST(CrcTest, CalculateCompareCombine)
{
    const char* data = "Hello, world!";
    const char* data1 = "Hello, ";
    const char* data2 = "world!";

    uint32_t crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(data), strlen(data));
    uint32_t crc1 = crc32(0L, Z_NULL, 0);
    crc1 = crc32(crc1, reinterpret_cast<const Bytef*>(data1), strlen(data1));
    uint32_t crc2 = crc32(0L, Z_NULL, 0);
    crc2 = crc32(crc2, reinterpret_cast<const Bytef*>(data2), strlen(data2));
    uint32_t combined_crc = crc32_combine(crc1, crc2, strlen(data2));
    printf("CRC32 of \"%s\": 0x%08X len %ld\n", data1, crc1, strlen(data));
    printf("CRC32 of \"%s\": 0x%08X len %ld\n", data2, crc2, strlen(data1));
    printf("Combined CRC32: 0x%08X len %ld\n", combined_crc, strlen(data2));
    
    ASSERT_EQ(crc, combined_crc);
}

TEST(CrcTest, CalculateInThreads)
{
    const std::string filename = "test_file.txt";
    ASSERT_TRUE(!populateFile(filename));

    const size_t chunkSize = 8192; // 8KB per chunk
    const size_t numOfThreads = 10; // 10 threads

    std::ifstream file(filename, std::ios::binary);
    ASSERT_TRUE(file.is_open());

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    char buffer[fileSize];
    file.read(buffer, fileSize);
    file.close();

    uint32_t crc1 = crc32(0L, Z_NULL, 0);
    crc1 = crc32(crc1, reinterpret_cast<const Bytef*>(buffer), fileSize);

    uint32_t crc2 = crc32(0L, Z_NULL, 0);
    crc2 = calculateCRC32Parallel(filename, chunkSize, numOfThreads);
    printf("CRC32 0x%08X\n", crc1);
    printf("CRC32 0x%08X\n", crc2);
    
    ASSERT_EQ(crc1, crc2);
}