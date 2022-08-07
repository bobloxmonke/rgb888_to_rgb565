#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

uint16_t rgb888_to_rgb565(uint8_t r, uint8_t g, uint8_t b, bool reverse_byte_order)
{
	uint16_t color = ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);

	if (reverse_byte_order)
	{
		color = ((color & 0x00ff) << 8) | ((color & 0xff00) >> 8); // _rotl16(color, 8) // _byteswap_ushort(color)
	}

	return color;
}

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("usage: rgb888_to_rgb565.exe [path to the image. example: C:\\Users\\some_user\\Desktop\\image.png] [out file name. example:file.txt] optional: [reverse byte order (0xFF00). example : true] \n");
		
		return 1;
	}

	bool reverse_byte_order = false;

	char* path = argv[1];
	char* out_path = argv[2];

	if (argc > 3)
	{
		if (strcmp(argv[3], "true") == 0 || atoi(argv[3]) == 1)
		{
			reverse_byte_order = true;
		}
	}

	int w, h, c;
	stbi_uc* image = stbi_load(path, &w, &h, &c, 3);

	if (image == NULL)
	{
		printf("out of memory!\n");

		return 0;
	}

	FILE* out;
	int result = fopen_s(&out, out_path, "w");

	if (result != 0)
	{
		printf("cannot open the file. please try again.");

		return 0;
	}

	fprintf(out, "// width: %d\n// height: %d\n// color format: RGB565 (16-bit color)\n\n", w, h);

	fprintf(out, "const uint16_t image[] = \n{\n");

	for (int y = 0; y < h; y++)
	{
		fprintf(out, "\t");

		for (int x = 0; x < w; x++)
		{
			uint8_t* pixel = &image[(x+y*w)*c];

			uint8_t r = pixel[0];
			uint8_t g = pixel[1];
			uint8_t b = pixel[2];

			fprintf(out, "0x%04x, ", rgb888_to_rgb565(r, g, b, reverse_byte_order));
		}

		fprintf(out, "\n");
	}

	fprintf(out, "};\n");

	free(image);
	fclose(out);

	printf("done!\n");

	system(out_path);

	return 0;
}