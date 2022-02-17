#define _CRT_SECURE_NO_WARNINGS

#include <direct.h>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

static const size_t c_1DMaxHz = 10;
static const size_t c_1DImageWidth = 120;
static const size_t c_1DImageHeight = 10;
static const size_t c_1DImagePaddingX = 1;
static const size_t c_1DImagePaddingY = 1;

static const float c_pi = 3.14159265359f;

template <typename T>
T Clamp(T value, T lo, T hi)
{
    if (value <= lo)
        return lo;
    else if (value >= hi)
        return hi;
    else
        return value;
}

int main(int argc, char** argv)
{
    _mkdir("out");

    // make the 1D images
    {
        const size_t c_actualImageWidth = c_1DImageWidth + c_1DImagePaddingX * 2;
        const size_t c_actualImageHeight = c_1DImageHeight * (c_1DMaxHz+1) + (c_1DMaxHz + 2) * c_1DImagePaddingY;
        std::vector<unsigned char> pixels(c_actualImageWidth * c_actualImageHeight, 0);

        for (size_t hz = 0; hz <= c_1DMaxHz; ++hz)
        {
            size_t imageStart = (hz * (c_1DImageHeight + c_1DImagePaddingY) + c_1DImagePaddingY) * c_actualImageWidth;

            for (size_t x = 0; x < c_1DImageWidth; ++x)
            {
                float p = float(x) / float(c_1DImageWidth - 1);

                float re = (cosf(0.0f) + cosf(2.0f * c_pi * float(hz) * p)) / 2.0f;
                float im = (sinf(0.0f) + sinf(2.0f * c_pi * float(hz) * p)) / 2.0f;

                float mag = sqrtf(re * re + im * im);
                unsigned char magc = (unsigned char)Clamp(mag * 256.0f, 0.0f, 255.0f);

                for (size_t y = 0; y < c_1DImageHeight; ++y)
                    pixels[imageStart + y * c_actualImageWidth + x + c_1DImagePaddingX] = magc;
            }
        }
        stbi_write_png("out/1d.png", (int)c_actualImageWidth, (int)c_actualImageHeight, 1, pixels.data(), 0);
    }

    return 0;
}