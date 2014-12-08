#include "image_tga.h"
#include <stdio.h>
#include <stdint.h>

namespace image_tga {

using namespace std;

#pragma pack(push, 1)
struct TgaHeader {
	uint8_t   id_length;
	uint8_t   colormap_type;
	uint8_t   image_type;
	uint16_t  colormap_index;
	uint16_t  colormap_length;
	uint8_t   colormap_size;
	uint16_t  x_origin;
	uint16_t  y_origin;
	uint16_t  width;
	uint16_t  height;
	uint8_t   pixel_size;
	uint8_t   flags;
};
#pragma pack(pop)

enum TGAType {
	TGA_TYPE_INDEXED            = 1,
	TGA_TYPE_RGB                = 2,
	TGA_TYPE_GREY               = 3,
	TGA_TYPE_RLE_INDEXED        = 9,
	TGA_TYPE_RLE_RGB            = 10,
	TGA_TYPE_RLE_GREY           = 11
};

#define TGA_INTERLEAVE_MASK     0xc0
#define TGA_INTERLEAVE_NONE     0x00
#define TGA_INTERLEAVE_2WAY     0x40
#define TGA_INTERLEAVE_4WAY     0x80

#define TGA_ORIGIN_MASK         0x30
#define TGA_ORIGIN_LEFT         0x00
#define TGA_ORIGIN_RIGHT        0x10
#define TGA_ORIGIN_LOWER        0x00
#define TGA_ORIGIN_UPPER        0x20

#define TGA_HAS_ALPHA           0x0F

bool save(const char* dst_path, const unsigned char* rgba, const unsigned w, const unsigned h) {
	TgaHeader header;
	header.id_length = 0;

	header.id_length = 0;
	header.colormap_type = 0;
	header.image_type = TGA_TYPE_RGB;

	header.colormap_index = 0;
	header.colormap_length = 0;
	header.colormap_size = 0;

	header.x_origin = 0;
	header.y_origin = 0;
	header.width = w;
	header.height = h;

	header.pixel_size = 32;
	header.flags = TGA_ORIGIN_UPPER | TGA_HAS_ALPHA;

	unsigned char* bgra = new unsigned char[w * h * 4];
	for (unsigned i = 0; i < w * h; ++i) {
		bgra[4 * i + 0] = rgba[4 * i + 2];
		bgra[4 * i + 1] = rgba[4 * i + 1];
		bgra[4 * i + 2] = rgba[4 * i + 0];
		bgra[4 * i + 3] = rgba[4 * i + 3];
	}

	FILE* f = fopen(dst_path, "wb");
	do {
		if (f == 0) {
			break;
		}
		fwrite(&header, sizeof(header), 1, f);
		fwrite(bgra, w * h * 4, 1, f);
	} while (0);
	delete[] bgra;
	fclose(f);
	return true;
}

} // namespace image_tga
