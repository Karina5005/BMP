#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

#define SIZEPIXEL 3
#define SIZEFILEHEADER 14
#define SIZEALIGN 4


int load_bmp(struct bmp* a, char* input_file) {
	FILE* input = fopen(input_file, "rb");
	if (input == NULL) {
		return 1;
	}
	fread(&a->file_header, sizeof(a->file_header), 1, input);
	fread(&a->info_header, sizeof(a->info_header), 1, input);

	a->img = malloc(a->info_header.height * sizeof(struct pixel*));
	if (a->img == NULL) {
		return 2;
	}
	for (int i = 0; i < a->info_header.height; i++) {
		a->img[i] = malloc(a->info_header.width * sizeof(struct pixel));
		if (a->img[i] == NULL) {
			return 3;
		}
	}

	fseek(input, a->file_header.offset_img, SEEK_SET);
	for (int i = a->info_header.height - 1; i >= 0; i--) {
		fread(a->img[i], a->info_header.width * sizeof(struct pixel), 1, input);
		fseek(input, (SIZEALIGN - (a->info_header.width * sizeof(struct pixel)) % SIZEALIGN) % SIZEALIGN, SEEK_CUR);
	}

	fclose(input);
	return 0;
}


int crop(struct bmp* a, int x, int y, int w, int h) {
	if (x + w < x || x + w > a->info_header.width || x + w < 0 || y + h < y || y + h > a->info_header.height || y + h < 0) {
		return -1;
	}

	struct bmp temp;
	temp.img = malloc(h * sizeof(struct pixel*));
	if (temp.img == NULL) {
		return -2;
	}
	for (int i = 0; i < h; i++) {
		temp.img[i] = malloc(w * sizeof(struct pixel));
		if (temp.img[i] == NULL) {
			return -3;
		}
		memcpy(temp.img[i], &(a->img[i + y][x]), w * sizeof(struct pixel));
	}
	free_bmp(a);
	a->img = temp.img;
	a->info_header.height = h;
	a->info_header.width = w;
	int align = (SIZEALIGN - (a->info_header.width * SIZEPIXEL) % SIZEALIGN) % SIZEALIGN;
	a->info_header.isize = a->info_header.height * (a->info_header.width * SIZEPIXEL + align);
	a->file_header.fsize = a->info_header.hsize + SIZEFILEHEADER + a->info_header.isize;
	return 0;
}


int rotate(struct bmp* a) {
	struct bmp temp;
	temp.img = malloc(a->info_header.width * sizeof(struct pixel*));
	if (temp.img == NULL) {
		return 1;
	}
	for (int i = 0; i < a->info_header.width; i++) {
		temp.img[i] = malloc(a->info_header.height * sizeof(struct pixel));
		if (temp.img[i] == NULL) {
			return 2;
		}
	}

	for (int i = 0; i < a->info_header.width; i++) {
		for (int j = a->info_header.height - 1; j >= 0; j--) {
			temp.img[i][a->info_header.height - 1 - j] = a->img[j][i];
		}
	}
	free_bmp(a);
	a->img = temp.img;
	int32_t tmp = a->info_header.width;
	a->info_header.width = a->info_header.height;
	a->info_header.height = tmp;
	int align = (SIZEALIGN - (a->info_header.width * SIZEPIXEL) % SIZEALIGN) % SIZEALIGN;
	a->info_header.isize = a->info_header.height * (a->info_header.width * SIZEPIXEL + align);
	a->file_header.fsize = a->info_header.hsize + SIZEFILEHEADER + a->info_header.isize;
	return 0;
}


int save_bmp(struct bmp* a, char* output_file) {
	FILE* output = fopen(output_file, "wb");
	fwrite(&a->file_header, sizeof(a->file_header), 1, output);
	fwrite(&a->info_header, sizeof(a->info_header), 1, output);
	int align = (SIZEALIGN - (a->info_header.width * sizeof(struct pixel)) % SIZEALIGN) % SIZEALIGN;
	uint32_t zeros = 0;
	for (unsigned int i = a->info_header.hsize + SIZEFILEHEADER; i < a->file_header.offset_img; i++) {
		fwrite(&zeros, 1, 1, output);
	}
	for (int i = a->info_header.height - 1; i >= 0; i--) {
		fwrite(a->img[i], a->info_header.width * sizeof(struct pixel), 1, output);
		fwrite(&zeros, align, 1, output);
	}
	fclose(output);
	return 0;
}

void free_bmp(struct bmp* a) {
	for (int i = 0; i < a->info_header.height; i++) {
		free(a->img[i]);
	}
	free(a->img);
}
