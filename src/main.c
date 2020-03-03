#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"


int main(int argc, char **argv) {
	argc = argc;

	struct bmp image;
	int test = load_bmp(&image, argv[2]);
	if (test != 0) {
		printf("Load error");
		free_bmp(&image);
		return -1;
	}
	if(strcmp("crop-rotate", argv[1]) == 0) {
		test = crop(&image, atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));
		if (test != 0) { 
			printf("Crop - error");
			free_bmp(&image);
			return -1; 
		}
		test = rotate(&image);
		if (test != 0) { 
			printf("Rotate - error");
			free_bmp(&image);
			return -2; 
			}
		test = save_bmp(&image, argv[3]);
		if (test != 0) { 
			printf("Save error");
			free_bmp(&image);
			return -3; 
		}
	}
	free_bmp(&image);
	return 0;
}

