#pragma once

namespace image_rgbe {

struct Data {
	float* rgb;
	unsigned w, h;
	bool valid;
};

Data load(const char* path);

} // namespace image_rgbe
