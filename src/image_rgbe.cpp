#include "image_rgbe.h"
#include <stdio.h>

extern "C" {
	#include "rgbe/rgbe.h"
}

namespace image_rgbe {

Data load(const char* path) {
	Data data = { 0, false };
	char * test = (char*)path;
	FILE* f = fopen( path, "rb");
	do {
		if (f == 0) {
			break;
		}

		rgbe_header_info header;
		int w, h;
		int result = RGBE_ReadHeader(f, &w, &h, &header);
		if (result == RGBE_RETURN_FAILURE) {
			break;
		}

		float* rgb = new float[3 * w * h];
		result = RGBE_ReadPixels_RLE(f, rgb, w, h);
		if (result == RGBE_RETURN_FAILURE) {
			break;
		}

		data.rgb = rgb;
		data.w = w;
		data.h = h;
		data.valid = true;
	} while(0);
	if (f) {
		fclose(f);
	}
	else {
		//std::cerr << "Cannot open the specified file, " << path << std::endl;
	}
	return data;
}

} // namespace image_rgbe
