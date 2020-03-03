#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)

struct BITMAPFILEHEADER {
	uint16_t sign; //signature 
	uint32_t fsize; //file size
	uint32_t r; //reserved
	uint32_t offset_img; //file offset to pixel array
};


struct BITMAPINFOHEADER {
	uint32_t hsize; //header size
	int32_t width; //image width
	int32_t height; //image height
	uint16_t p; //planes
	uint16_t bits_pix; //bits per pixel
	uint32_t comp; //compression
	uint32_t isize; //image size
	int32_t x; //pixels per meter
	int32_t y; //pixels per meter
	uint32_t col; //colors in color table
	uint32_t imp; //important color count
};

#pragma pack(pop)

struct pixel {
	char first;
	char second;
	char third;
};

struct bmp {
	struct BITMAPFILEHEADER file_header;
	struct BITMAPINFOHEADER info_header;
	struct pixel **img;
};

int load_bmp(struct bmp* a, char* input_file);

int crop(struct bmp* a, int x, int y, int w, int h);

int rotate(struct bmp* a);

int save_bmp(struct bmp* a, char* output_file);

void free_bmp(struct bmp* a);
