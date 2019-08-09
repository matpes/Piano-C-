#pragma once
#include "Formatter.h"

#include <fstream>
#include <vector>
#include <stdexcept>

#pragma pack(push, 1)
struct BMPFileHeader {
	uint16_t file_type{ 0x4D42 };          // File type always BM which is 0x4D42 (stored as hex uint16_t in little endian)
	uint32_t file_size{ 0 };               // Size of the file (in bytes)
	uint16_t reserved1{ 0 };               // Reserved, always 0
	uint16_t reserved2{ 0 };               // Reserved, always 0
	uint32_t offset_data{ 0 };             // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
	uint32_t size{ 0 };                      // Size of this header (in bytes)
	int32_t width{ 0 };                      // width of bitmap in pixels
	int32_t height{ 0 };                     // width of bitmap in pixels
											 //       (if positive, bottom-up, with origin in lower left corner)
											 //       (if negative, top-down, with origin in upper left corner)
	uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
	uint16_t bit_count{ 0 };                 // No. of bits per pixel
	uint32_t compression{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
	uint32_t size_image{ 0 };                // 0 - for uncompressed images
	int32_t x_pixels_per_meter{ 0 };
	int32_t y_pixels_per_meter{ 0 };
	uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
	uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
};
#pragma pack(pop)

struct BMP {
	BMPFileHeader file_header;
	BMPInfoHeader bmp_info_header;
	std::vector<uint8_t> data;



	

	BMP(int32_t width, int32_t height) {
		if (width <= 0 || height <= 0) {
			throw std::runtime_error("The image width and height must be positive numbers.");
		}
		bmp_info_header.width = width;
		bmp_info_header.height = height;

		bmp_info_header.size = sizeof(BMPInfoHeader);
		file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

		bmp_info_header.bit_count = 24;
		bmp_info_header.compression = 0;
		row_stride = width * 3;
		data.resize(row_stride * height);

		uint32_t new_stride = make_stride_aligned(4);
		file_header.file_size = file_header.offset_data + data.size() + bmp_info_header.height * (new_stride - row_stride);

	}

	void write(const char *fname) {
		std::ofstream of{ fname, std::ios_base::binary };
		if (of) {

			if (bmp_info_header.width % 4 == 0) {
				write_headers_and_data(of);
			}
			else {
				uint32_t new_stride = make_stride_aligned(4);
				std::vector<uint8_t> padding_row(new_stride - row_stride);

				write_headers(of);

				for (int y = 0; y < bmp_info_header.height; ++y) {
					of.write((const char*)(data.data() + row_stride * y), row_stride);
					of.write((const char*)padding_row.data(), padding_row.size());
				}
			}
		}
		else {
			throw std::runtime_error("Unable to open the output image file.");
		}
	}

	void fill_region(Composition *c);



private:
	uint32_t row_stride{ 0 };

	void write_headers(std::ofstream &of) {
		of.write((const char*)&file_header, sizeof(file_header));
		of.write((const char*)&bmp_info_header, sizeof(bmp_info_header));
	}

	void write_headers_and_data(std::ofstream &of) {
		write_headers(of);
		of.write((const char*)data.data(), data.size());
	}

	// Add 1 to the row_stride until it is divisible with align_stride
	uint32_t make_stride_aligned(uint32_t align_stride) {
		uint32_t new_stride = row_stride;
		while (new_stride % align_stride != 0) {
			new_stride++;
		}
		return new_stride;
	}
	void determine_colors(uint8_t &R, uint8_t &G, uint8_t &B, MusicSymbol *ms);

};




class BMPFormatter :
	public Formatter
{
private :
	BMP * bmp;
	Composition *c;
	
public:
	BMPFormatter() :Formatter() {}
	void createFile(string x) override;
	virtual ~BMPFormatter();
};

