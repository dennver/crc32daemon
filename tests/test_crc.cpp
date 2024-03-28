#include "../app/crc32.h"
#include <zlib.h>
#include <gtest/gtest.h>

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
