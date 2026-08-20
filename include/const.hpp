#ifndef __CONST_HPP__
#define __CONST_HPP__

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

inline float PixelsToMeters(float pixels)
{
    float PIXELS_PER_METER = 100.0f;
    return pixels / PIXELS_PER_METER;
}

inline float MetersToPixels(float meters)
{
    float PIXELS_PER_METER = 100.0f;
    return meters * PIXELS_PER_METER;
}
#endif //__CONST_HPP__