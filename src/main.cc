#include <iostream>
#include <filesystem>
#include <string_view>

#include <stdint.h>

uint32_t hash(const void* data, uintmax_t data_len)
{
	if (!data) return 0;
	int32_t hash = 0x55555555;

	for (uintmax_t i = 0; i < data_len; ++i)
		hash = (hash >> 27) + (hash << 5) + static_cast<const uint8_t*>(data)[i];

	return hash;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cout << "please enter the file name" << std::endl;
		return -1;
	}

	std::string path = argv[1];

	if (!std::filesystem::exists(path))
	{
		std::cout << "that file doesn't exist" << std::endl;
		return -1;
	}

	uint64_t size = std::filesystem::file_size(path);
	uint8_t* data = static_cast<uint8_t*>(malloc(size));

	std::cout << "file hash: " << hash(data, size) << std::endl;

	return 0;
}