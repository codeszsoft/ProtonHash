#include <iostream>
#include <filesystem>
#include <string>
#include <string_view>

#include <stdint.h>

uint32_t hash(const void* data, uintmax_t len)
{
	if (!data)
		return 0;

	uint32_t hash_ = 0x55555555;
	for (uintmax_t i = 0; i < len; ++i)
		hash_ = (hash_ >> 27) + (hash_ << 5) + static_cast<const uint8_t*>(data)[i];
	
	return hash_;
}

uint8_t* get_data(const std::string& file, size_t* size)
{
	uint8_t* data = NULL;

	FILE* open;

#ifdef _WIN32
	fopen_s(&open, file.c_str(), "rb");
#else
	fopen(&open, file.c_str(), "rb");
#endif

	if (!open) 
		return 0;

	fseek(open, 0, SEEK_END);
	*size = ftell(open);
	fseek(open, 0, SEEK_SET);

	data = (uint8_t*)new uint8_t[((*size) + 1)];

	if (!data)
		std::cout << "Out of memory while opening " << file << "?" << std::endl;

	data[*size] = 0;

	fread(data, *size, 1, open);
	fclose(open);

	return data;
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
	uint8_t* data = get_data(path, &size);

	std::cout << "file hash: " << hash(data, size) << std::endl;

	return 0;
}
