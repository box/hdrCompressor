#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "image_rgbe.h"
#include "image_tga.h"

typedef unsigned int uint;
typedef unsigned char uchar;

class Application {
public:
	
	bool convert(const char* src_path, const char* dst_path, const char* format) {
		const image_rgbe::Data rgbe = image_rgbe::load(src_path);
		if (!rgbe.valid) {
			std::cerr << "Can not load source image." << std::endl;
			return false;
		}
		
		uint dataSize = rgbe.w * rgbe.h * 4;
		uchar * compressedData = (uchar *)malloc(dataSize);

		if (strcmp("-rgbm", format) == 0)
		{
			compressRGBM(rgbe, compressedData);
		}
		else if (std::strcmp( "-rgbd", format) == 0)
		{
			compressRGBD(rgbe, compressedData);
		}
		else if (std::strcmp( "-rgbe", format) == 0)
		{
			compressRGBE(rgbe, compressedData);
		}
		else if (std::strcmp( "-logluv", format) == 0)
		{
		}
		else {
			std::cerr << "Unknown compression format, " << format << std::endl;
		}

		if ( !image_tga::save(dst_path, compressedData, rgbe.w, rgbe.h) ) {
			return false;
		}

		return true;
	}
private:
	float fclampf(float n, float lower, float upper) {
		return fmaxf(lower, fminf(n, upper));
	}
	void compressRGBM(const image_rgbe::Data & rgbe, uchar * newImage ) {

		float threshold = 1e-6f;
		float range = 9.0;

		float * inPixel = rgbe.rgb;
		uchar * outPixel = newImage;
		
		float maxFloat = 0.0f;
		uchar maxInt = 0;
		const uint count = rgbe.w * rgbe.h;
		for (uint i = 0; i < count; i++) {
			
			float R = *inPixel++;
			float G = *inPixel++;
			float B = *inPixel++;
			R /= range;
			G /= range;
			B /= range;
			float M = fclampf(fmaxf(fmaxf(R, G), fmaxf(B, threshold)), 0.0f, 1.0f);
			M = ceilf(M * 255.0f) / 255.0f;
			if (M > maxFloat) {
				maxFloat = M;
			}
			*outPixel++ = (uchar)(255.0f * fclampf(R / M, 0.0f, 1.0f));
			*outPixel++ = (uchar)(255.0f * fclampf(G / M, 0.0f, 1.0f));
			*outPixel++ = (uchar)(255.0f * fclampf(B / M, 0.0f, 1.0f));
			*outPixel = (uchar)(255.0f * M);
			if (*outPixel > maxInt) {
				maxInt = *outPixel;
			}
			outPixel++;
		}
		printf("Max float is %f", maxFloat);
		printf("Max int is %uc", maxInt);

	}

	void compressRGBD(const image_rgbe::Data & rgbe, uchar * newImage) {

		float threshold = 1e-6f;
		
		float * inPixel = rgbe.rgb;
		uchar * outPixel = newImage;

		float maxFloat = 0.0f;
		uchar maxInt = 0;
		const uint count = rgbe.w * rgbe.h;
		for (uint i = 0; i < count; i++) {
			float R = *inPixel++;
			float G = *inPixel++;
			float B = *inPixel++;
			float M = fmaxf(fmaxf(R, G), fmaxf(B, 1.0));
			if (M > maxFloat) {
				maxFloat = M;
			}
			*outPixel++ = (uchar)(255.0f * fclampf(R / M, 0.0f, 1.0f));
			*outPixel++ = (uchar)(255.0f * fclampf(G / M, 0.0f, 1.0f));
			*outPixel++ = (uchar)(255.0f * fclampf(B / M, 0.0f, 1.0f));
			*outPixel = (uchar)(255.0f / M);
			if (*outPixel > maxInt) {
				maxInt = *outPixel;
			}
			outPixel++;
		}
		printf("Max float is %f", maxFloat);
		printf("Max int is %uc", maxInt);
	}

	void compressRGBE(const image_rgbe::Data & rgbe, uchar * newImage) {

		float threshold = 1e-6f;

		float * inPixel = rgbe.rgb;
		uchar * outPixel = newImage;

		const uint count = rgbe.w * rgbe.h;
		for (uint i = 0; i < count; i++) {
			float R = *inPixel++;
			float G = *inPixel++;
			float B = *inPixel++;
			float M = fmaxf(fmaxf(R, G), fmaxf(B, 0.0));
			if (M < 1e-24) M = 0.0;

			//Based on http://www.graphics.cornell.edu/~bjw/rgbe/rgbe.c
			
			float exponent = ceilf(log2(M));
			float value = exp2f(-exponent);

			*outPixel++ = (uchar)(255.0f * fclampf(R * value, 0.0f, 1.0f));
			*outPixel++ = (uchar)(255.0f * fclampf(G * value, 0.0f, 1.0f));
			*outPixel++ = (uchar)(255.0f * fclampf(B * value, 0.0f, 1.0f));
			*outPixel++ = (uchar)(exponent + 128.0f);
			
		}
	}

	//void convertToLOGLUV(nv::FloatImage * image, nv::Image * newImage) {
	//}
	
};

int main(int argc, char* argv[]) {
	if (argc != 4) {
		std::cout << "hdrCompressor by Mike Bond (michael.bond@verold.com)" << std::endl;
		std::cout << "Based on hdr2rgbm 1.0-rc1 by Denis Mentey (denis@goortom.com)" << std::endl;
		std::cout << "Usage:" << std::endl;
		std::cout << "hdrCompress.exe <format> <hdr-image> <tga-output>" << std::endl;
		std::cout << "Where <format> is -rgbe, -rgbm, -rgbd, or -logluv" << std::endl;
		return 1;
	}
	
	Application app;
	
	return app.convert(argv[2], argv[3], argv[1] ) ? 0 : 1;
}
