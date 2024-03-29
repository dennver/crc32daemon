#include <stdint.h>
#include <stddef.h>
#include <string>

#pragma once

unsigned int crc32Function(unsigned char *buf, unsigned long len);
uint32_t crc32Combine(uint32_t crc1, uint32_t crc2, uint32_t len2);
uint32_t crc32WithTable(char* buf, size_t len);

uint32_t calculateCRC32Parallel(const std::string& filename, size_t chunkSize, uint32_t threadCount);
