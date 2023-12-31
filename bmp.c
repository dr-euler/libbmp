#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>

#define WRITE_2BYTES(val) fprintf(img->outfile, "%c%c", (val), (val)>>8)
#define WRITE_3BYTES(val) fprintf(img->outfile, "%c%c%c", (val), (val)>>8, (val)>>16) 
#define WRITE_4BYTES(val) fprintf(img->outfile, "%c%c%c%c", (val), (val)>>8, (val)>>16, (val)>>24)

void bmp_init(bmp* img, char* outfile_name, unsigned int width, unsigned int height){
	if( !(img->outfile = fopen(outfile_name, "w")) ){
		fprintf(stderr, "Couldn't open file\n");
		exit(1);
	}
	img->width = width;
	img->height = height;
}

void bmp_write_header(bmp* img){
	// offset for bit data 26
	
	const unsigned int size = 26 + img->width*img->height*3; //assuming 24bits (3bytes) per pixel	
	const unsigned int offset = 26;
	char buff[27];
	sprintf(buff, "BM%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
			size, size>>8, size>>16, size>>24,
			'\0', '\0', '\0', '\0',
			offset, offset>>8, offset>>16, offset>>24,
			12U, 12U>>8, 12U>>16, 12U>>24,
			img->width, img->width>>8,
			img->height, img->height>>8,
			1U, 1U>>8,
			24U, 24U>>8);

	fwrite(buff, 1, 26, img->outfile);
}

void bmp_free(bmp* img){
	fclose(img->outfile);
}

void bmp_buff_init(bmp_buff* buff, bmp* img, char* pixbuff){
	buff->img = img;
	buff->pixbuff = pixbuff;
	buff->size = 0;
}

void bmp_buff_reset(bmp_buff* buff){
	buff->size = 0;
}

void bmp_buff_push_pixel(bmp_buff* buff, unsigned int pix){
	buff->pixbuff[buff->size++] = (char)(pix>>0);
	buff->pixbuff[buff->size++] = (char)(pix>>8);
	buff->pixbuff[buff->size++] = (char)(pix>>16);
}

void bmp_buff_write(bmp_buff* buff){
	fwrite(buff->pixbuff, 1, buff->size, buff->img->outfile);
}

